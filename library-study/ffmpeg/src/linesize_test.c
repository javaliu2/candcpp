#include <stdio.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixfmt.h>
/**
 * YUV420
 * YYYY
 * YYYY
 * 
 * UU
 * 
 * VV
 */
int main() {
    int width = 640;
    int height = 480;
    enum AVPixelFormat pix_fmt = AV_PIX_FMT_YUV420P;

    uint8_t *data[4] = {0};
    int linesize[4] = {0};

    // align是指按照多少字节对齐
    int ret = av_image_alloc(data, linesize, width, height, pix_fmt, 32);
    if (ret < 0) {
        printf("av_image_alloc failed\n");
        return -1;
    }

    printf("Y linesize = %d\n", linesize[0]);  // 640
    printf("U linesize = %d\n", linesize[1]);  // 320
    printf("V linesize = %d\n", linesize[2]);  // 320

    // 640 * 480 = 0x4B000
    // 320 * 240 = 0x12C00
    // U和V平面的宽度和高度均减半
    // Y: 640 * 480, U, V: 320 * 240
    printf("Y data = %p\n", data[0]);  // 0x77f050ea8040 + 0x4B000 = 0x77f050ef3040
    printf("U data = %p\n", data[1]);  // 0x77f050ef3040 + 0x12C00 = 0x77f050f05c40
    printf("V data = %p\n", data[2]);  // 0x77f050f05c40

    // 遍历Y平面
    for (int i = 0; i < height; ++i) {
        uint8_t* row_start = data[0] + i * linesize[0];
        for (int j = 0; j < width; ++j) {
            printf("0x%02x", row_start[j]);
        }
        printf("\n");
        break;
    }
    av_freep(&data[0]);
    return 0;
}