#pragma once
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
class ThreadPool {
private:
    bool stop;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex my_mutex;
    std::condition_variable cv;
public:
    ThreadPool();
    ThreadPool(int size);
    ~ThreadPool();
    void addTask(std::function<void()> task);
};