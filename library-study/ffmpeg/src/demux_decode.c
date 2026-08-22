/**
 * 说明如何使用 libavformat 和 libavcodec API 来解复用和解码音频和视频数据。
 * mux: multiplex, 多重通道
 * Write the output as raw audio and input files to be played by
 * ffplay？
 * 输出是原始音频和原始视频，文件可以被ffplay播放。
 * 
 */

#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <stdio.h>

static AVFormatContext *fmt_ctx = NULL;
static AVCodecContext *video_dec_ctx = NULL, *audio_dec_ctx = NULL;
static int width, height;
static enum AVPixelFormat pix_fmt;
static AVStream *video_stream = NULL, *audio_stream = NULL;
static const char *src_filename = NULL;
static const char *video_dst_filename = NULL;
static const char *audio_dst_filename = NULL;
static FILE *video_dst_file = NULL;
static FILE *audio_dst_file = NULL;

static uint8_t *video_dst_data[4] = {NULL};
static int  video_dst_linesize[4];
static int video_dst_bufsize;

static int video_stream_idx = -1, audio_stream_idx = -1;
static AVFrame *frame = NULL;
static AVPacket *pkt = NULL;
static int video_frame_count = 0;
static int audio_frame_count = 0;
static int print_video_frame_linesize = 0;

// 将 视频frame 保存到 rawvideo 文件
static int output_video_frame(AVFrame *frame) {
    if (frame->width != width || frame->height != height || frame->format != pix_fmt) {
        fprintf(stderr, "Error: Width, height and pixel format have to be "
        "constant in a rawvideo file, but the width, height or pixel format "
        "of the input video changed:\n"
        "old: width = %d, height = %d, format = %s\n"
        "new: width = %d, height = %d, format = %s\n",
            width, height, av_get_pix_fmt_name(pix_fmt),
            frame->width, frame->height, av_get_pix_fmt_name(frame->format));
        return -1;
    }
    if (!print_video_frame_linesize) {
        for (int i = 0; i < 3; ++i) {
            printf("frame plane %d linesize: %d\n", i, frame->linesize[i]);
        }
        /**
         * frame plane 0 linesize: 1920
         * frame plane 1 linesize: 960
         * frame plane 2 linesize: 960
         * 可以发现，源frame的linesize和video_dst_linesize是一致的
         */
        print_video_frame_linesize = 1;
    }
    printf("video frame n:%d\n", video_frame_count++);
    /**
     * copy decoded frame to destination buffer:
     * this is required since rawvideo expects non aligned data
     */
    // 所谓non aligned数据，就是每一个plane去除padding，剩下的的有效数据
    // g-bro: 根据源 frame 的 linesize，把每个 plane 的有效区域逐行复制到 destination
    av_image_copy2(video_dst_data, video_dst_linesize, frame->data, frame->linesize, pix_fmt, width, height);
    /* write to rawvideo file */
    // video_dst_data 和 video_dst_bufsize 在 av_image_alloc() 被设置，align为1
    fwrite(video_dst_data[0], 1, video_dst_bufsize, video_dst_file);
    // 这里写入的是YUV三个平面，因为video_dst_data[0]决定了这一帧数据的起始地址，video_dst_bufsize决定了这一帧的大小
    // 由于是non-align，所以内存是连续的。
    return 0;
}

// 将 音频frame 保存到 rawaudio 文件
static int output_audio_frame(AVFrame *frame) {
    // audio是一维数据，所以linesize就是plane数据的大小
    size_t unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample(frame->format);
    printf("audio frame count: %d nb_samples:%d pts:%s\n",
    audio_frame_count++, frame->nb_samples, av_ts2timestr(frame->pts, &audio_dec_ctx->time_base));
    /**
     * write the raw audio samples of the first plane.
     * this works fine for packed formats (e.g. AV_SAMPLE_FMT_S16).
     * however, most audio decoders output planar audio, which uses a
     * seperate planes of audio samples for each channel(e.g. AV_SANPLE_FMT_S16P).
     * in other words, this code will write only the first audio channel in these cases.
     * you should use libswresample or libavfilter to convert the frame to packed data.
     */
    // 只保存第一个plane的数据，即只保存一个声道的数据
    // packed格式的包含所有声道的数据，例如2声道的话，就是L0R0 L1R1 ... L1023R1023
    fwrite(frame->extended_data[0], 1, unpadded_linesize, audio_dst_file);
    return 0;
}

// 根据dec, 将pkt解码为frame, 然后调用output_video/audio_frame(frame)
static int decode_packet(AVCodecContext *dec, const AVPacket *pkt) {
    int ret = 0;
    // submit the packet to the decoder
    ret = avcodec_send_packet(dec, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error submitting a packet for decoding (%s)\n", av_err2str(ret));
        return ret;
    }
    // get all the available frames from the decoder
    while (ret >= 0) {
        ret = avcodec_receive_frame(dec, frame);
        if (ret < 0) {
            // those two return values are special and mean there is no output
            // frame available, but there were no errors during decoding
            if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN)) {
                return 0;
            }
            fprintf(stderr, "Error during decoding (%s)\n", av_err2str(ret));
            return ret;
        }
        // write the frame data to output file
        if (dec->codec->type == AVMEDIA_TYPE_VIDEO) {
            ret = output_video_frame(frame);
        } else {
            ret = output_audio_frame(frame);
        }
        av_frame_unref(frame);
    }
    return ret;
}

// 查找 fmt_ctx 中 type 类型的流
// 根据流查找解码器，创建解码器context，初始化其context为流中codec参数
// 初始化解码器
static int open_codec_context(int *stream_idx, AVCodecContext **dec_ctx, AVFormatContext *fmt_ctx, enum AVMediaType type) {
    int ret, stream_index;
    AVStream *st;
    const AVCodec *dec = NULL;

    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %s stream in input file '%s'\n", 
        av_get_media_type_string(type), src_filename);
        return ret;
    } else {
        stream_index = ret;
        st = fmt_ctx->streams[stream_index];
        /* find decoder for the stream */
        dec = avcodec_find_decoder(st->codecpar->codec_id);
        printf("[open_codec_context] found decoder: %s\n", dec->name);  // h264 aac
        if (!dec) {
            fprintf(stderr, "Failed to find %s codec\n", av_get_media_type_string(type));
            return AVERROR(EINVAL);
        }
        /* allocate a codec context for the decoder */
        // 通过二级指针修改一级指针变量的值
        *dec_ctx = avcodec_alloc_context3(dec);
        if (!*dec_ctx) {
            fprintf(stderr, "Failed to allcate the %s codec context\n", 
            av_get_media_type_string(type));
            return AVERROR(ENOMEM);
        }
        /* copy codec parameters from input stream to output codec context */
        if ((ret = avcodec_parameters_to_context(*dec_ctx, st->codecpar)) < 0) {
            fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
            av_get_media_type_string(type));
            return ret;
        }
        /* init the decoders */
        if((ret = avcodec_open2(*dec_ctx, dec, NULL)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n", 
            av_get_media_type_string(type));
            return ret;
        }
        *stream_idx = stream_index;
    }
    return 0;
}

// 获取 sample_fmt 的fmt_be, fmt_le
static int get_format_from_sample_fmt(const char **fmt, enum AVSampleFormat sample_fmt) {
    int i;
    struct sample_fmt_entry {
        enum AVSampleFormat sample_fmt; const char *fmt_be, *fmt_le;
    } sample_fmt_entries[] = {
        {AV_SAMPLE_FMT_U8, "u8", "u8"},
        {AV_SAMPLE_FMT_S16, "s16be", "s16le"},
        {AV_SAMPLE_FMT_S32, "s32be", "s32le"},
        {AV_SAMPLE_FMT_FLT, "f32be", "f32le"},
        {AV_SAMPLE_FMT_DBL, "f62be", "f64le"}, 
    };
    // 这几个sample_fmt不带P(planar)，那就是packed格式
    // be: big endian，大端
    // le: little endian，小端
    *fmt = NULL;
    for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); ++i) {
        struct sample_fmt_entry *entry = &sample_fmt_entries[i];
        if (sample_fmt == entry->sample_fmt) {
            *fmt = AV_NE(entry->fmt_be, entry->fmt_le);
            return 0;
        }
    }
    fprintf(stderr, "sample format %s is not supported as output format\n",
    av_get_sample_fmt_name(sample_fmt));
    return -1;
}

int main(int argc, char** argv) {
    int ret = 0;
    if (argc != 4) {
        fprintf(stderr, "usage: %s input_file video_output_file audio_output_file\n"
        "API example program to show how to read frames from an input file.\n"
        "This program read frames from a file, decodes them, and write decoded\n"
        "video frames to a rawvideo file named video_ouput_file, and decoded\n"
        "audio frames to a rawaudio file named auido_output_file.\n");
        exit(1);
    }
    src_filename = argv[1];
    video_dst_filename = argv[2];
    audio_dst_filename = argv[3];

    /* open input file, and allocate format context */
    if (avformat_open_input(&fmt_ctx, src_filename, NULL, NULL) < 0) {
        fprintf(stderr, "Failed to open source file %s\n", src_filename);
        exit(1);
    }
    /* retrieve stream infomation */
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        exit(1);
    }
    if (open_codec_context(&video_stream_idx, &video_dec_ctx, fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0) {
        video_stream = fmt_ctx->streams[video_stream_idx];
        video_dst_file = fopen(video_dst_filename, "wb");
        if (!video_dst_file) {
            fprintf(stderr, "Could not open destination file %d\n", video_dst_filename);
            ret = 1;
            goto end;
        }
        /* allocte image where the decoded image will be put */
        width = video_dec_ctx->width;
        height = video_dec_ctx->height;
        pix_fmt = video_dec_ctx->pix_fmt;

        // last para是align，其值设置为1，表示linesize大小为有效数据大小，没有padding用来内存对齐
        // 1字节对齐，那么对齐结果就是本来有的数据，有多少就是多少，不会为了align而使用padding进行对齐
        printf("before av_image_alloc, video_dst_linesize: \n");
        for (int i = 0; i < 4; ++i) {
            printf("plane %d, linesize: %d, video_dst_data: %p\n", i, video_dst_linesize[i], video_dst_data[i]);
        }
        /*
        * plane 0, linesize: 0, video_dst_data: (nil)
        * plane 1, linesize: 0, video_dst_data: (nil)
        * plane 2, linesize: 0, video_dst_data: (nil)
        * plane 3, linesize: 0, video_dst_data: (nil)
        */
        ret = av_image_alloc(video_dst_data, video_dst_linesize, width, height, pix_fmt, 1);
        printf("after  av_image_alloc, video_dst_linesize: \n");
        for (int i = 0; i < 4; ++i) {
            printf("plane %d, linesize: %d, video_dst_data: %p\n", i, video_dst_linesize[i], video_dst_data[i]);
        }
        /**
         * plane 0, linesize: 1920, video_dst_data: 0x70abb1308040
         * plane 1, linesize: 960, video_dst_data: 0x70abb1502440 = 0x70abb1308040 + 1920*1080(dec)
         * plane 2, linesize: 960, video_dst_data: 0x70abb1580d40 = 0x70abb1502440 + 960*540(dec)
         * plane 3, linesize: 0, video_dst_data: (nil)
         */
        if (ret < 0) {
            fprintf(stderr, "Could not allocate raw video buffer\n");
            goto end;
        }
        video_dst_bufsize = ret;
        // 0x70abb1308040(video_dst_data[0]) + 2F7600(video_dst_bufsize) ?= 0x70abb1580d40(video_dst_data[2]) + 960*540(dec)
        // 70ABB15FF640 ?= 70ABB15FF640, yes! 
        printf("video_dst_bufsize: %d\n", video_dst_bufsize);  // 1920*1080*3/2==3110400(dec)=2F7600(hex)
        // print: video_dst_bufsize: 3110400
    }
    if (open_codec_context(&audio_stream_idx, &audio_dec_ctx, fmt_ctx, AVMEDIA_TYPE_AUDIO) >= 0) {
        audio_stream = fmt_ctx->streams[audio_stream_idx];
        audio_dst_file = fopen(audio_dst_filename, "wb");
        if(!audio_dst_file) {
            fprintf(stderr, "Could not open destination file %d\n", audio_dst_filename);
            ret = 1;
            goto end;
        }
    }

    /* dump input information to stderr */
    av_dump_format(fmt_ctx, 0, src_filename, 0);
    /**
     * Input #0, mov,mp4,m4a,3gp,3g2,mj2, from 'test.mp4':
  Metadata:
    major_brand     : isom
    minor_version   : 512
    compatible_brands: isomiso2avc1mp41
    comment         : vid:v1e00fgi0000d913ad7og65nfjps5uq0
    encoder         : Lavf58.76.100
  Duration: 00:00:17.53, start: 0.000000, bitrate: 1861 kb/s
  Stream #0:0[0x1](und): Audio: aac (HE-AACv2) (mp4a / 0x6134706D), 44100 Hz, stereo, fltp, 63 kb/s (default)
    Metadata:
      handler_name    : SoundHandler
  Stream #0:1[0x2](und): Video: h264 (High) (avc1 / 0x31637661), yuv420p(tv, bt709, progressive), 1920x1080 [SAR 1:1 DAR 16:9], 1790 kb/s, 30 fps, 30 tbr, 15360 tbn (default)
    Metadata:
      handler_name    : VideoHandler
     */

    if (!audio_stream && !video_stream) {
        fprintf(stderr, "Could not find audio or video stream in the input, aborting\n");
        ret = 1;
        goto end;
    }
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate frame\n");
        ret = AVERROR(ENOMEM);
        goto end;
    }
    pkt = av_packet_alloc();
    if (!pkt) {
        fprintf(stderr, "Could not allocate packet\n");
        ret = AVERROR(ENOMEM);
        goto end;
    }
    if (video_stream) {
        printf("[main] Demuxing video from file '%s' into '%s'\n", src_filename, video_dst_filename);
    }
    if (audio_stream) {
        printf("[main] Demuxing audio from file '%s' into '%s'\n", src_filename, audio_dst_filename);
    }
    /* read frames from the file */
    while (av_read_frame(fmt_ctx, pkt) >= 0) {
        // check if the packet belongs to a stream we are interested in, otherwise skip it
        if (pkt->stream_index == video_stream_idx) {
            ret = decode_packet(video_dec_ctx, pkt);
        } else if (pkt->stream_index == audio_stream_idx) {
            ret = decode_packet(audio_dec_ctx, pkt);
        }
        av_packet_unref(pkt);
        if (ret < 0) {
            break;
        }
    }
    /* flush the decoders */
    if (video_dec_ctx) {
        decode_packet(video_dec_ctx, NULL);
    }
    if (audio_dec_ctx) {
        decode_packet(audio_dec_ctx, NULL);
    }
    printf("Demuxing successed.\n");
    if (video_stream) {
        printf("Play the output video file with the command:\n"
        "ffplay -f rawvideo -pixel_format %s -video_size %dx%d %s\n",
        av_get_pix_fmt_name(pix_fmt), width, height, video_dst_filename);
    }
    if (audio_stream) {
        enum AVSampleFormat sfmt = audio_dec_ctx->sample_fmt;
        AVChannelLayout mono = AV_CHANNEL_LAYOUT_MONO;
        AVChannelLayout *ch_layout = &audio_dec_ctx->ch_layout;
        const char *fmt;
        char buf[64];
        // 没有进行planar->packed的转换，这里只是设置用户读取rawaudio文件的ffplay参数
        if (av_sample_fmt_is_planar(sfmt)) {
            const char *packed = av_get_sample_fmt_name(sfmt);
            printf("Warning: the sample format the docoder produced is planar "
            "(%s). This is example will output the first channel only.\n", 
            packed ? packed : "?");
            sfmt = av_get_packed_sample_fmt(sfmt);  // 获取sfmt的packed格式
            ch_layout = &mono;  // 设置layout为单声道
        // Warning: the sample format the docoder produced is planar (fltp). This is example will output the first channel only.
        // fltp: float32 planar，32位浮点数+planar(每个声道单独存储)
        }
        if((ret = get_format_from_sample_fmt(&fmt, sfmt)) < 0) {
            goto end;
        }
        if ((ret = av_channel_layout_describe(ch_layout, buf, sizeof(buf))) < 0) {
            goto end;
        }
        printf("Play the output audio file with the command:\n"
        "ffplay -f %s -ch_layout %s -sample_rate %d %s\n",
        fmt, buf, audio_dec_ctx->sample_rate, audio_dst_filename);
    }
end:
    avcodec_free_context(&video_dec_ctx);
    avcodec_free_context(&audio_dec_ctx);
    avformat_close_input(&fmt_ctx);
    if (video_dst_file) {
        fclose(video_dst_file);
    }
    if (audio_dst_file) {
        fclose(audio_dst_file);
    }
    av_packet_free(&pkt);
    av_frame_free(&frame);
    av_free(video_dst_data[0]);
    return ret < 0;
}

