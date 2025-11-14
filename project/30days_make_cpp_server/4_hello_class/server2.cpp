#include "EventLoop.h"
#include "Server.h"
#include <stdio.h>
#include <fcntl.h>
#include <functional>

int main() {
    EventLoop loop;
    Server server(&loop);

    /**
     * 将服务器标准输入内容 回显到所有client连接
     */
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);  // 必须按 Enter 才会触发一次 EPOLLIN，所以不必担心单个字符就需要回显
    Channel channel(&loop, STDIN_FILENO);
    std::function<void()> echo_all_func = std::bind(&Server::echoAll, &server);  // 需要取地址，因为this就是指针嘛
    channel.setCallback(echo_all_func);
    channel.enableReading();

    loop.loop();
    return 0;
}