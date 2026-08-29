/**
 * ffmpeg official example, study by xs.
 * @file libavcodec audio decoding API usage example
 * @example decode_audio.c
 *
 * Decode data from an MP2 input file and generate a raw audio file to be played with ffplay.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <libavcodec/avcodec.h>

#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096
FILE *log_file = NULL;
int log_i;
int64_t audio_frame_idx = 0;

static int get_format_from_sample_fmt(const char **fmt,
                                      enum AVSampleFormat sample_fmt)
{
    int i;
    struct sample_fmt_entry
    {
        enum AVSampleFormat sample_fmt;
        const char *fmt_be, *fmt_le;
    } sample_fmt_entries[] = {
        {AV_SAMPLE_FMT_U8, "u8", "u8"},  // 一个字节不涉及大端或小端
        {AV_SAMPLE_FMT_S16, "s16be", "s16le"},
        {AV_SAMPLE_FMT_S32, "s32be", "s32le"},
        {AV_SAMPLE_FMT_FLT, "f32be", "f32le"},
        {AV_SAMPLE_FMT_DBL, "f64be", "f64le"},
    };
    *fmt = NULL;

    for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++)
    {
        struct sample_fmt_entry *entry = &sample_fmt_entries[i];
        if (sample_fmt == entry->sample_fmt)
        {
            *fmt = AV_NE(entry->fmt_be, entry->fmt_le);
            return 0;
        }
    }

    fprintf(stderr,
            "sample format %s is not supported as output format\n",
            av_get_sample_fmt_name(sample_fmt));
    return -1;
}

static void decode(AVCodecContext *dec_ctx, AVPacket *pkt, AVFrame *frame, FILE *outfile)
{
    int i, ch;
    int ret, data_size;

    /* send the packet with the compressed data to the decoder */
    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0)
    {
        fprintf(stderr, "Error submitting the packet to the decoder\n");
        exit(1);
    }
    /* read all the output frames (in general there may be any number of them) */
    fprintf(log_file, "==round %d: ==\n", log_i++);
    while (ret >= 0)
    {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            break;
        }
        else if (ret < 0)
        {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }
        // s16p, AV_SAMPLE_FMT_S16P, 16bits, 故2字节
        data_size = av_get_bytes_per_sample(dec_ctx->sample_fmt);
        printf("data_size: %d\n", data_size);  // 2
        fprintf(log_file,
        "frame=%" PRId64
        ", pts=%" PRId64
        ", nb_samples=%d"
        ", sample_rate=%d"
        ", data_size=%d\n",
        audio_frame_idx++,
        frame->pts,
        frame->nb_samples,
        frame->sample_rate,
        data_size);
        if (data_size < 0)
        {
            /* this should not occur, checking just for paranoia(多疑、无端恐惧) */
            fprintf(stderr, "Failed to calculate data size\n");
            exit(1);
        }
        printf("nb_samples: %d\n", frame->nb_samples);  // 1152
        printf("nb_channels: %d\n", dec_ctx->ch_layout.nb_channels);  // 2
        // 将 planar 格式的输入数据，手工输出为 packed 格式的保存数据
        // data[0]: L0 L1 ... L1023
        // data[1]: R0 R1 ... R1023
        // 写入文件的数据: L0 R0 L1 R1 ... L1023 R1023
        for (i = 0; i < frame->nb_samples; ++i)
        {
            for (ch = 0; ch < dec_ctx->ch_layout.nb_channels; ++ch)
            {
                fwrite(frame->data[ch] + data_size * i, 1, data_size, outfile);
            }
        }
    }
    fprintf(log_file, "=====end=====\n");
}

int main(int argc, char **argv)
{   
    const char *log_filename = "decode_audio_log";
    log_file = fopen(log_filename, "w+");
    log_i = 0;

    if (!log_file) {
        fprintf(stderr, "open %s failed.\n", log_filename);
        exit(1);
    }
    const char *outfilename, *filename;
    const AVCodec *codec = NULL;
    AVCodecContext *codec_ctx = NULL;
    AVCodecParserContext *parser = NULL;

    int len, ret;
    FILE *f, *outfile;
    uint8_t inbuf[AUDIO_INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    uint8_t *data;
    size_t data_size;
    AVPacket *pkt;
    AVFrame *frame = NULL;
    enum AVSampleFormat sfmt;
    int n_channels = 0;
    const char *fmt;

    if (argc <= 2)
    {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }
    filename = argv[1];
    outfilename = argv[2];

    pkt = av_packet_alloc();
    if (!pkt)
    {
        fprintf(stderr, "Could not allocate AVPacket\n");
        exit(1); /* or proper cleanup and returning */
    }
    /* find the MPEG audio decoder */
    codec = avcodec_find_decoder(AV_CODEC_ID_MP2);
    if (!codec)
    {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }
    parser = av_parser_init(codec->id);
    if (!parser)
    {
        fprintf(stderr, "Parser not found\n");
        exit(1);
    }
    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx)
    {
        fprintf(stderr, "Could not allocate audio codec context\n");
        exit(1);
    }
    /* open it */
    if (avcodec_open2(codec_ctx, codec, NULL) < 0)
    {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }
    f = fopen(filename, "rb");
    if (!f)
    {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }
    outfile = fopen(outfilename, "wb");
    if (!outfile)
    {
        fprintf(stderr, "Could not open %s\n", outfilename);
        exit(1);
    }
    /* decode until eof */
    data = inbuf;
    data_size = fread(inbuf, 1, AUDIO_INBUF_SIZE, f);
    while (data_size > 0)
    {
        if (!frame)
        {
            if (!(frame = av_frame_alloc()))
            {
                fprintf(stderr, "Could not allocate audio frame\n");
                exit(1);
            }
        }
        // function: Parse a packet. 解析出来一个packet
        // return: the number of bytes of the input bitstream used.
        // 消耗的输入码流字节数量
        ret = av_parser_parse2(parser, codec_ctx, &pkt->data, &pkt->size, data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
        if (ret < 0)
        {
            fprintf(stderr, "Error while parsing\n");
            exit(1);
        }
        data += ret;  // 要读取数据的位置
        data_size -= ret;  // 剩余数据的大小
        if (pkt->size)
        {
            decode(codec_ctx, pkt, frame, outfile);
        }
        // AUDIO_REFILL_THRESH 是对 inbuf 写入新数据的阈值
        // THRESH是阈值threshold的缩写
        if (data_size < AUDIO_REFILL_THRESH)
        {
            memmove(inbuf, data, data_size);  // 从data拷贝data_size个字节到inbuf
            // 即将剩余的data_size个数据从inbuf末尾拷贝到inbuf头部
            // 那么[inbuf + data_size, ... end)区间就可以写入新数据了
            data = inbuf;  // 读取数据指针置为inbuf头
            // 可用缓冲区大小: AUDIO_INBUF_SIZE - data_size
            len = fread(data + data_size, 1, AUDIO_INBUF_SIZE - data_size, f);
            if (len > 0)
            {
                data_size += len;  // 修改剩余数据的大小
            }
        }
    }
    /* flush the docoder */
    pkt->data = NULL;
    pkt->size = 0;
    decode(codec_ctx, pkt, frame, outfile);

    /* print output pcm infomations, because there have no metadata of pcm */
    sfmt = codec_ctx->sample_fmt;
    if (av_sample_fmt_is_planar(sfmt))
    {
        const char *planar_fmt = av_get_sample_fmt_name(sfmt);
        printf("Warning: the sample format the decoder produced is planar "
               "(%s). This example will output all the channel with packed format manually.\n",
               planar_fmt ? planar_fmt : "?");  // s16p
        sfmt = av_get_packed_sample_fmt(sfmt);
        const char *packed_fmt = av_get_sample_fmt_name(sfmt);
        printf("after av_get_packed_sample_fmt(), sfmt: %s\n", packed_fmt);  // s16
    }
    n_channels = codec_ctx->ch_layout.nb_channels;
    if ((ret = get_format_from_sample_fmt(&fmt, sfmt)) < 0)
    {
        goto end;
    }
    printf("Play the output audio file with the command: \n"
           "ffplay -f %s -ch_layout %s -ar %d %s \n",
           fmt, n_channels == 1 ? "mono" : "stereo", codec_ctx->sample_rate, outfilename);
end:
    fclose(outfile);
    fclose(f);
    avcodec_free_context(&codec_ctx);
    av_parser_close(parser);
    av_frame_free(&frame);
    av_packet_free(&pkt);
    return 0;
}
