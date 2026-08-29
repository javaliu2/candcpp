/**
 * @file libavcodec encoding video API usage example
 * @example encode_video.c
 *
 * Generate synthetic video data and encode it to an output file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
static const char *my_picture_type_name(enum AVPictureType type)
{
    switch (type)
    {
    case AV_PICTURE_TYPE_I:
        return "I";
    case AV_PICTURE_TYPE_P:
        return "P";
    case AV_PICTURE_TYPE_B:
        return "B";
    default:
        return "OTHER";
    }
}

static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt, FILE *outfile)
{
    int ret;
    /* send the frame to the encoder */
    // if (frame)
    // {
    //     printf("send frame pts=%3" PRId64
    //            " type=%s\n",
    //            frame->pts,
    //            my_picture_type_name(frame->pict_type));
    // }
    ret = avcodec_send_frame(enc_ctx, frame);
    if (frame)
    {
        printf("send frame pts=%3" PRId64
               " type=%s\n",
               frame->pts,
               my_picture_type_name(frame->pict_type));
    }
    if (ret < 0)
    {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }
    while (ret >= 0)
    {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            return;
        }
        else if (ret < 0)
        {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }
        printf("Write packet pts=%3" PRId64
               " dts=%3" PRId64
               " key=%d"
               " size=%5d\n",
               pkt->pts,
               pkt->dts,
               !!(pkt->flags & AV_PKT_FLAG_KEY),
               pkt->size);
        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }
}

int main(int argc, char **argv)
{
    const char *filename, *codec_name;
    const AVCodec *codec;
    AVCodecContext *c = NULL;
    int i, ret, x, y;
    FILE *f;
    AVFrame *frame;
    AVPacket *pkt;
    uint8_t endcode[] = {0, 0, 1, 0xb7};

    if (argc <= 2)
    {
        fprintf(stderr, "Usage: %s <output file> <codec name>\n", argv[0]);
        exit(0);
    }
    filename = argv[1];
    codec_name = argv[2];
    /* find the mpeg1video encoder */
    codec = avcodec_find_encoder_by_name(codec_name);
    if (!codec)
    {
        fprintf(stderr, "Codec '%s' not found\n", codec_name);
        exit(1);
    }
    c = avcodec_alloc_context3(codec);
    if (!c)
    {
        fprintf(stderr, "Could not allocate video codec context");
        exit(1);
    }
    pkt = av_packet_alloc();
    if (!pkt)
    {
        exit(1);
    }
    /* put sample parameters */
    c->bit_rate = 400000;
    /* resolution must be a multiple of two */

    const char *input_filename = "/home/xs/Documents/candcpp/library-study/ffmpeg/resource/rawvideo";
    enum AVPixelFormat src_pix_fmt = AV_PIX_FMT_YUV420P;
    FILE *input_file = NULL;
    input_file = fopen(input_filename, "rb");
    if (!input_file)
    {
        fprintf(stderr, "open %s failed", input_filename);
        exit(1);
    }
    int width = 1920;
    int height = 1080;
    int frame_size = av_image_get_buffer_size(src_pix_fmt, width, height, 1);
    uint8_t *yuv_buffer = malloc(frame_size);
    if (!yuv_buffer)
    {
        perror("malloc");
        fclose(input_file);
        return -1;
    }
    c->width = width;
    c->height = height;
    /* frames per second */
    c->time_base = (AVRational){1, 25};
    c->framerate = (AVRational){25, 1};

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame to encoder,
     * if frame->pict_type is AV_PICTURE_TYPE_I,
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec->id == AV_CODEC_ID_H264)
    {
        av_opt_set(c->priv_data, "preset", "slow", 0);
    }
    /* open it */
    ret = avcodec_open2(c, codec, NULL);
    if (ret < 0)
    {
        fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
        exit(1);
    }
    f = fopen(filename, "wb");
    if (!f)
    {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }

    frame = av_frame_alloc();
    if (!frame)
    {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->format = c->pix_fmt;
    frame->width = c->width;
    frame->height = c->height;

    ret = av_frame_get_buffer(frame, 0); // 2nd是align，传递0表示自动设置当前CPU适应的align
    if (ret < 0)
    {
        fprintf(stderr, "Could not allocate the video frame data\n");
        exit(1);
    }
    /* encode 1 second of video */
    for (i = 0; i < 25; ++i)
    {
        fflush(stdout);
        /* Make sure the frame data is writable.
         * On the first round, the frame is fresh from av_frame_get_buffer()
         * and therefore we know it is writable.
         * But on the next rounds, encode() will have called
         * avcodec_send_frame(), and the codec may have kept a reference to the frame
         * in its internal structures, that makes the frame unwritable.
         * av_frame_make_writable() checks that and allocates a new buffer for
         * the frame only if necessary.
         *
         */
        ret = av_frame_make_writable(frame);
        if (ret < 0)
        {
            exit(1);
        }
        /* prepare a dummy image.
         * In real code, this is where you would have your own logic for filling the frame.
         * FFmpeg does not care what you put in the frame.
         */
        ret = fread(yuv_buffer, 1, frame_size, input_file);
        if (ret != frame_size)
        {
            printf("读取失败，实际读取 %d bytes\n", ret);
            free(yuv_buffer);
            fclose(input_file);
            return -1;
        }
        // printf("读取一帧成功，共 %d bytes\n", ret);
        uint8_t *Y = yuv_buffer;
        uint8_t *U = Y + width * height;
        uint8_t *V = U + width * height / 4;
        /* Y */
        for (y = 0; y < c->height; ++y)
        {
            for (x = 0; x < c->width; ++x)
            {
                frame->data[0][y * frame->linesize[0] + x] = Y[y * c->width + x];
            }
        }
        /* Cb and Cr */
        // Cb: 描述蓝色相对于亮度(Y)的差异
        // Cr: 描述红色相对于亮度(Y)的差异
        for (y = 0; y < c->height / 2; ++y)
        {
            for (x = 0; x < c->width / 2; ++x)
            {
                frame->data[1][y * frame->linesize[1] + x] = U[y * (c->width / 2) + x];
                frame->data[2][y * frame->linesize[2] + x] = V[y * (c->width / 2) + x];
            }
        }
        frame->pts = i;
        /* encode the image */
        encode(c, frame, pkt, f);
    }
    /* flush the encoder */
    encode(c, NULL, pkt, f);
    /*
     * Add sequence end code to have a real MPEG file.
     * It makes only sense because this tiny examples writes packets directly.
     * This is called "elementary stream" and only works for some codecs.
     * To create a vaild file, you usually need to write packets into a proper
     * file format or protocol; see mux.c.
     */
    // elementary stream: 基本码流
    if (codec->id == AV_CODEC_ID_MPEG1VIDEO || codec->id == AV_CODEC_ID_MPEG2VIDEO)
    {
        fwrite(endcode, 1, sizeof(endcode), f);
    }
    fclose(f);
    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);
    return 0;
}