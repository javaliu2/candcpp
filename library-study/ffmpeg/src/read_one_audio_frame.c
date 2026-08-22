#include <stdio.h>
#include <stdlib.h>

int main() {
    const int nb_samples = 2048;  // 一个声道一帧的采样数
    const int channels = 1;  // 单声道，mono
    const int bytes_per_sample = sizeof(float);  // 每个采样点大小

    const int frame_bytes = nb_samples * channels * bytes_per_sample;
    printf("nb_samples = %d\n", nb_samples);
    printf("channels   = %d\n", channels);
    printf("frame size = %d bytes\n", frame_bytes);

    const char *filename = "/home/xs/Documents/candcpp/library-study/ffmpeg/resource/rawaudio";
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("fopen");
        return -1;
    }
    float *samples = malloc(frame_bytes);
    if (!samples) {
        perror("malloc");
        fclose(fp);
        return -1;
    }
    // 读取一帧音频数据
    size_t ret = fread(samples, sizeof(float), nb_samples, fp);
    if (ret != nb_samples) {
        printf("读取失败: 只读取了 %zu 个sample\n", ret);
        free(samples);
        fclose(fp);
        return -1;
    }
    printf("成功读取，读取了 %zu 个sample\n", ret);
    for (int i = 0;i < 10; ++i) {
        printf("sample[%d] = %f\n", i, samples[i]);
    }
    free(samples);
    fclose(fp);
    return 0;
}