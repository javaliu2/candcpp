/**
 *
 * 拉取 IPC 摄像机的视频流，实时展示
 *
 */

extern "C"
{
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <SDL2/SDL.h>

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

// =========================
// 全局共享状态
// =========================

std::atomic<bool> running(true);

std::mutex frame_mutex;

// 解码线程产生的最新 frame
AVFrame *shared_frame = nullptr;

// 是否有新 frame
bool frame_ready = false;

// =========================
// FFmpeg 相关
// =========================

AVFormatContext *fmt_ctx = nullptr;
AVCodecContext *codec_ctx = nullptr;

int video_stream_index = -1;

// =========================
// 拉流 + 解码线程
// =========================

void decode_thread()
{
    AVPacket *pkt = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();

    if (!pkt || !frame)
    {
        cerr << "allocate packet/frame failed\n";
        running = false;
        return;
    }

    while (running)
    {
        int ret = av_read_frame(fmt_ctx, pkt);

        if (ret < 0)
        {
            cerr << "av_read_frame failed\n";
            break;
        }

        // 只处理视频流
        if (pkt->stream_index == video_stream_index)
        {
            ret = avcodec_send_packet(codec_ctx, pkt);

            if (ret < 0)
            {
                cerr << "avcodec_send_packet failed\n";

                av_packet_unref(pkt);
                continue;
            }

            while (running)
            {
                ret = avcodec_receive_frame(codec_ctx, frame);

                if (ret == AVERROR(EAGAIN) ||
                    ret == AVERROR_EOF)
                {
                    break;
                }

                if (ret < 0)
                {
                    cerr << "avcodec_receive_frame failed\n";
                    break;
                }

                // =========================
                // 得到一个完整 AVFrame
                // =========================

                {
                    std::lock_guard<std::mutex> lock(frame_mutex);

                    if (shared_frame == nullptr)
                    {
                        shared_frame = av_frame_alloc();
                    }

                    // 释放 shared_frame 原来的引用
                    av_frame_unref(shared_frame);

                    // 引用当前 frame 的数据
                    av_frame_ref(shared_frame, frame);

                    frame_ready = true;
                }
            }
        }

        av_packet_unref(pkt);
    }

    av_frame_free(&frame);
    av_packet_free(&pkt);

    running = false;
}

// =========================
// main
// =========================

int main()
{
    const char *url =
        "rtsp://admin:woshishuia123@192.168.31.164:554/stream2";

    // ========================================
    // 1. 打开 RTSP
    // ========================================

    AVDictionary *opts = nullptr;

    av_dict_set(&opts, "rtsp_transport", "tcp", 0);
    av_dict_set(&opts, "stimeout", "5000000", 0);

    int ret = avformat_open_input(
        &fmt_ctx,
        url,
        nullptr,
        &opts);

    av_dict_free(&opts);

    if (ret < 0)
    {
        cerr << "open " << url << " failed.\n";
        return -1;
    }

    // ========================================
    // 2. 获取流信息
    // ========================================

    ret = avformat_find_stream_info(
        fmt_ctx,
        nullptr);

    if (ret < 0)
    {
        cerr << "find stream failed\n";

        avformat_close_input(&fmt_ctx);

        return -1;
    }

    // ========================================
    // 3. 找视频流
    // ========================================

    for (unsigned int i = 0; i < fmt_ctx->nb_streams; ++i)
    {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream_index = i;
            break;
        }
    }

    if (video_stream_index == -1)
    {
        cerr << "could not find video stream\n";

        avformat_close_input(&fmt_ctx);

        return -1;
    }

    AVStream *video_stream =
        fmt_ctx->streams[video_stream_index];

    // ========================================
    // 4. 获取 width / height
    // ========================================

    int width = video_stream->codecpar->width;

    int height = video_stream->codecpar->height;

    cout << "video size: "
         << width
         << "x"
         << height
         << endl;

    // ========================================
    // 5. 创建 decoder
    // ========================================

    const AVCodec *decoder =
        avcodec_find_decoder(
            video_stream->codecpar->codec_id);

    if (!decoder)
    {
        cerr << "find decoder failed\n";

        avformat_close_input(&fmt_ctx);

        return -1;
    }

    codec_ctx = avcodec_alloc_context3(decoder);

    if (!codec_ctx)
    {
        cerr << "allocate codec context failed\n";

        avformat_close_input(&fmt_ctx);

        return -1;
    }

    ret = avcodec_parameters_to_context(
        codec_ctx,
        video_stream->codecpar);

    if (ret < 0)
    {
        cerr << "init codec_context para from decoder failed\n";

        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);

        return -1;
    }

    ret = avcodec_open2(
        codec_ctx,
        decoder,
        nullptr);

    if (ret < 0)
    {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];

        av_strerror(
            ret,
            errbuf,
            sizeof(errbuf));

        cerr << "avcodec_open2 failed: "
             << errbuf
             << endl;

        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);

        return -1;
    }

    // ========================================
    // 6. 初始化 SDL
    // ========================================

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cerr << "SDL_Init failed: "
             << SDL_GetError()
             << endl;

        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);

        return -1;
    }

    SDL_Window *window =
        SDL_CreateWindow(
            "IPC",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_SHOWN);

    if (!window)
    {
        cerr << "SDL_CreateWindow failed: "
             << SDL_GetError()
             << endl;

        SDL_Quit();

        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);

        return -1;
    }

    SDL_Renderer *renderer =
        SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        cerr << "SDL_CreateRenderer failed: "
             << SDL_GetError()
             << endl;

        SDL_DestroyWindow(window);
        SDL_Quit();

        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);

        return -1;
    }

    // ========================================
    // 7. 创建 YUV Texture
    // ========================================

    SDL_Texture *texture =
        SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_IYUV,
            SDL_TEXTUREACCESS_STREAMING,
            width,
            height);

    if (!texture)
    {
        cerr << "SDL_CreateTexture failed: "
             << SDL_GetError()
             << endl;

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);

        return -1;
    }

    // ========================================
    // 8. 启动拉流 + 解码线程
    // ========================================

    std::thread decoder_thread(
        decode_thread);

    // ========================================
    // 9. SDL 主线程
    // ========================================

    while (running)
    {
        // ------------------------------------
        // 处理 SDL 事件
        // ------------------------------------

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                cout << "window close" << endl;

                running = false;
            }

            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }
        }

        // ------------------------------------
        // 获取最新 frame
        // ------------------------------------

        AVFrame *local_frame = nullptr;

        {
            std::lock_guard<std::mutex> lock(frame_mutex);

            if (frame_ready)
            {
                local_frame = av_frame_alloc();

                if (local_frame)
                {
                    // 增加引用
                    av_frame_ref(
                        local_frame,
                        shared_frame);

                    frame_ready = false;
                }
            }
        }

        // ------------------------------------
        // 显示 frame
        // ------------------------------------

        if (local_frame)
        {
            cout << "frame: "
                 << local_frame->width
                 << "x"
                 << local_frame->height
                 << " format="
                 << local_frame->format
                 << endl;

            SDL_UpdateYUVTexture(
                texture,
                nullptr,

                // Y
                local_frame->data[0],
                local_frame->linesize[0],

                // U
                local_frame->data[1],
                local_frame->linesize[1],

                // V
                local_frame->data[2],
                local_frame->linesize[2]);

            SDL_RenderClear(renderer);

            SDL_RenderCopy(
                renderer,
                texture,
                nullptr,
                nullptr);

            SDL_RenderPresent(renderer);

            av_frame_free(&local_frame);
        }

        // 防止主线程空转占满 CPU
        SDL_Delay(1);
    }

    // ========================================
    // 10. 通知解码线程退出
    // ========================================

    running = false;

    // 等待解码线程结束
    decoder_thread.join();

    // ========================================
    // 11. 清理共享 frame
    // ========================================

    {
        std::lock_guard<std::mutex> lock(frame_mutex);

        if (shared_frame)
        {
            av_frame_free(&shared_frame);
        }
    }

    // ========================================
    // 12. 清理 SDL
    // ========================================

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    // ========================================
    // 13. 清理 FFmpeg
    // ========================================

    avcodec_free_context(&codec_ctx);

    avformat_close_input(&fmt_ctx);

    return 0;
}