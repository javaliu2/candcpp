#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include "ClientSocket.h"
#include "InetAddress.h"
#include "ThreadPool.h"
#include <iostream>
#include "Logger.h"
#include <vector>
#include <string>
#include <random>
#include <thread>

std::string generate_sentence() {
    std::vector<std::string> subjects = {
        "我", "你", "他", "程序员", "猫", "机器人"
    };

    std::vector<std::string> verbs = {
        "喜欢", "正在学习", "讨厌", "研究", "理解", "忽略"
    };

    std::vector<std::string> objects = {
        "C++", "操作系统", "佛法", "强化学习", "服务器编程", "代码"
    };

    std::vector<std::string> adverbs = {
        "在深夜", "在实验室里", "在咖啡馆", "默默地", "认真地", "反复地"
    };

    std::random_device rd;
    std::mt19937 gen(rd());

    auto pick = [&](const std::vector<std::string>& v) {
        std::uniform_int_distribution<> dist(0, v.size() - 1);
        return v[dist(gen)];
    };

    std::string sentence =
        pick(subjects) + pick(verbs) + pick(objects) + pick(adverbs) + "。";
    return sentence;
}
void fun() {
    InetAddress target_addr("127.0.0.1", 9999);
    ClientSocket client(target_addr);
    // 生成1-5之间的某个随机数x，生成x个句子，每隔随机2-4秒发送给服务器
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> count_dist(1, 5);
    int x = count_dist(gen);
    std::uniform_int_distribution<> sleep_dist(2, 4);

    char buf[1024];
    for (int i = 0; i < x; ++i) {
        std::string sentence = generate_sentence();
        int fd = client.getFd();
        std::string msg = "client " + std::to_string(fd) + ": " + sentence;
        client.write(msg.c_str(), msg.size());
        Logger::log("client send: ", msg);
        // std::cout << "thread_id: " << std::this_thread::get_id() << " has send: " << msg << "\n";
        int n = client.read(buf, sizeof(buf));
        std::string replay(buf, n);
        Logger::log("client read: ", replay);
        // std::cout << "thread_id: " << std::this_thread::get_id() << " has read: " << buf << "\n";
        if (i + 1 < x) {
            int sec = sleep_dist(gen);
            std::this_thread::sleep_for(std::chrono::seconds(sec));
        }
    }
}
int main () {
    const int size = 8;
    ThreadPool threadPool(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> sleep_dist(2, 4);
    for (int i = 0; i < size; ++i) {
        threadPool.addTask(fun);
        // int sec = sleep_dist(gen);
        // std::this_thread::sleep_for(std::chrono::seconds(sec));
    }
    return 0;
}