/**
 * 从 rawvideo yuv420p 1920*1080 文件中读取一帧，
 * 使用ffmpeg库函数将这一帧数据转化成一张图片
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>


int main() {
    const char *input_file = "/home/xs/Documents/candcpp/library-study/ffmpeg/resource/rawvideo";
    const char *output_file = "/home/xs/Documents/candcpp/library-study/ffmpeg/resource/frame.jpg";
    enum AVPixelFormat src_pix_fmt = AV_PIX_FMT_YUV420P;
    enum AVPixelFormat dst_pix_fmt = AV_PIX_FMT_YUVJ420P;
    int width = 1920;
    int height = 1080;

    // YUV420P 一帧大小
    int frame_size = width * height * 3 / 2;  // 3110400
    printf("frame_size: %d\n", frame_size);

    FILE *fp = fopen(input_file, "rb");
    if (!fp) {
        perror("fopen");
        return -1;
    }
    uint8_t *yuv_buffer = malloc(frame_size);
    if (!yuv_buffer) {
        perror("malloc");
        fclose(fp);
        return -1;
    }
    // 读取一帧
    // 每个元素的大小为size字节，元素的个数为n个
    size_t ret = fread(yuv_buffer, 1, frame_size, fp);
    if (ret != frame_size) {
        printf("读取失败，实际读取 %zu bytes\n", ret);
        free(yuv_buffer);
        fclose(fp);
        return -1;
    }
    printf("读取一帧成功，共 %zu bytes\n", ret);
    uint8_t *Y = yuv_buffer;  // 0x7f2fdb908010
    uint8_t *U = Y + width * height;  // 0x7f2fdbb02410
    uint8_t *V = U + width * height / 4;  // 0x7f2fdbb80d10

    printf("Y = %p\n", Y);
    printf("U = %p\n", U);
    printf("V = %p\n", V);

    // 打印前几个 Y 数据
    for (int i = 0; i < 10; i++) {
        printf("Y[%d] = 0x%02x ", i, Y[i]);
    }
    printf("\n");
    // 打印前几个 U 数据
    for (int i = 0; i < 10; i++) {
        printf("U[%d] = 0x%02x ", i, U[i]);
    }
    printf("\n");
    // 打印前几个 V 数据
    for (int i = 0; i < 10; i++) {
        printf("V[%d] = 0x%02x ", i, V[i]);
    }
    printf("\n");

    // 4、创建 YUV420P AVFrame
    AVFrame *src_frame = av_frame_alloc();
    if (!src_frame) {
        printf("av_frame_alloc failed\n");
        free(yuv_buffer);
        fclose(fp);
        return -1;
    }

    src_frame->format = AV_PIX_FMT_YUV420P;
    src_frame->width = width;
    src_frame->height = height;
    printf("\n===== before fill, YUV420P =====\n");
    printf("Y data      = %p\n",
           (void *)src_frame->data[0]);  // nil

    printf("U data      = %p\n",
           (void *)src_frame->data[1]);  // nil 

    printf("V data      = %p\n",
           (void *)src_frame->data[2]);  // nil

    printf("Y linesize  = %d\n",
           src_frame->linesize[0]);  // 0

    printf("U linesize  = %d\n",
           src_frame->linesize[1]);  // 0

    printf("V linesize  = %d\n",
           src_frame->linesize[2]);  // 0

    // 5、让 ffmpeg 根据 YUV420P 的格式计算 data[] 和 linesize

    // 根据width、height、和pix_fmt解析src指向的内存，填充frame的data和linesize变量
    ret = av_image_fill_arrays(src_frame->data, src_frame->linesize, yuv_buffer, src_pix_fmt, width, height, 1);
    if (ret < 0) {
        printf("av_image_fill_arrays failed\n");
        av_frame_free(&src_frame);
        free(yuv_buffer);
        fclose(fp);
        return -1;
    }
    // 5.1、打印 YUV420P 的内存布局
    printf("\n===== YUV420P =====\n");
    printf("Y data      = %p\n",
           (void *)src_frame->data[0]);  // 0x7f2fdb908010

    printf("U data      = %p\n",
           (void *)src_frame->data[1]);

    printf("V data      = %p\n",
           (void *)src_frame->data[2]);

    printf("Y linesize  = %d\n",
           src_frame->linesize[0]);  // 1920

    printf("U linesize  = %d\n",
           src_frame->linesize[1]);  // 960
    printf("V linesize  = %d\n",
           src_frame->linesize[2]);


    // 6、创建 SwsContext，负责 将YUV420P -> YUVJ420P，JPEG编码器可以直接接受 YUVJ420P
    struct SwsContext *sws_ctx = sws_getContext(
        width, height, src_pix_fmt,
        width, height, dst_pix_fmt,
        SWS_BILINEAR, NULL, NULL, NULL
    );
    if (!sws_ctx) {
        printf("sws_getContext failed\n");
        av_frame_free(&src_frame);
        free(yuv_buffer);
        fclose(fp);
        return -1;
    }
 
    // 7、创建 JPEG AVFrame
    AVFrame *dst_frame = av_frame_alloc();
    if (!dst_frame) {
        printf("av_frame_alloc dst_frame failed\n");

        sws_freeContext(sws_ctx);
        av_frame_free(&src_frame);
        free(yuv_buffer);
        fclose(fp);

        return -1;
    }
    dst_frame->format = dst_pix_fmt;
    dst_frame->width = width;
    dst_frame->height = height;
    printf("before get buffer, dst_frame->data: %p\n", dst_frame->data);
    /**
     * 为 dst_frame 分配内存
     */
    ret = av_frame_get_buffer(dst_frame, 1);
    if (ret < 0) {
        printf("av_frame_get_buffer failed\n");

        av_frame_free(&dst_frame);
        sws_freeContext(sws_ctx);
        av_frame_free(&src_frame);
        free(yuv_buffer);
        fclose(fp);

        return -1;
    }
    printf("after  get buffer, dst_frame->data: %p\n", dst_frame->data);
    // 8、YUV420P -> YUVJ420P
    // sws_scale()会根据src_frame->data和src_frame->linesize读取源图像

    // 这个二级指针的双const修饰看不懂
    const uint8_t *const *p = (const uint8_t *const*)src_frame->data;
    const uint8_t *const p1;
    p = &p1;  // 二级指针本身可以修改指向
    // *p 是 const uint8_t *const类型的指针，它不能改变指向，不能修改所指向的数据
    // *p = p1;  // error: assignment of read-only location ‘*p’
    // *p[0] = 0x12;  // error: assignment of read-only location ‘*(const uint8_t *)*p’
    // return -1;
    sws_scale(sws_ctx,
    (const uint8_t *const*)src_frame->data, src_frame->linesize,
    0, height, dst_frame->data, dst_frame->linesize);

    // 9、找 JPEG 编码器
    const AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
    if (!codec) {
        printf("mjpeg encoder not found\n");

        av_frame_free(&dst_frame);
        sws_freeContext(sws_ctx);
        av_frame_free(&src_frame);
        free(yuv_buffer);
        fclose(fp);

        return -1;
    }

    // 10、创建 JPEG 编码器上下文
    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) {
        printf("avcodec_alloc_context3 failed\n");

        av_frame_free(&dst_frame);
        sws_freeContext(sws_ctx);
        av_frame_free(&src_frame);
        free(yuv_buffer);
        fclose(fp);

        return -1;
    }
    codec_ctx->width = width;
    codec_ctx->height = height;
    codec_ctx->pix_fmt = dst_pix_fmt;
    codec_ctx->time_base = (AVRational){1, 25};

    codec_ctx->global_quality = 2;

    // 11、打开 JPEG 编码器
    ret = avcodec_open2(codec_ctx, codec, NULL);
    if (ret < 0) {
        printf("avcodec_open2 failed\n");

        avcodec_free_context(&codec_ctx);
        av_frame_free(&dst_frame);
        sws_freeContext(sws_ctx);
        av_frame_free(&src_frame);
        free(yuv_buffer);
        fclose(fp);

        return -1;
    }

    // 12、打开输出 JPEG 文件
    FILE *jpg = fopen(output_file, "wb");
    if (!jpg) {
        perror("fopen output");

        avcodec_free_context(&codec_ctx);
        av_frame_free(&dst_frame);
        sws_freeContext(sws_ctx);
        av_frame_free(&src_frame);
        free(yuv_buffer);
        fclose(fp);

        return -1;
    }

    // 13、把 AVFrame 送入 JPEG 编码器
    ret = avcodec_send_frame(codec_ctx, dst_frame);
    if (ret < 0) {
        printf("avcodec_send_frame failed\n");

        fclose(jpg);
        avcodec_free_context(&codec_ctx);
        av_frame_free(&dst_frame);
        sws_freeContext(sws_ctx);
        av_frame_free(&src_frame);
        free(yuv_buffer);
        fclose(fp);

        return -1;
    }
    // 14、获取 JPEG packet
    AVPacket *packet = av_packet_alloc();
    if (!packet) {
        printf("av_packet_alloc failed\n");

        fclose(jpg);
        avcodec_free_context(&codec_ctx);
        av_frame_free(&dst_frame);
        sws_freeContext(sws_ctx);
        av_frame_free(&src_frame);
        free(yuv_buffer);
        fclose(fp);

        return -1;
    }
    ret = avcodec_receive_packet(codec_ctx, packet);
    if (ret == 0) {
        // JPEG 数据就在packet->data
        fwrite(packet->data, 1, packet->size, jpg);
        printf("\nJPEG size=%d bytes\n", packet->size);
        printf("save image: %s\n", output_file);
    } else {
        printf("avcodec_receive_packet failed\n");
    }

    av_packet_free(&packet);
    fclose(jpg);
    avcodec_free_context(&codec_ctx);
    av_frame_free(&dst_frame);
    sws_freeContext(sws_ctx);
    av_frame_free(&src_frame);
    free(yuv_buffer);
    fclose(fp);
    return 0;
}