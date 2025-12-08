#include "ThreadPool.h"
#include <thread>
#include <iostream>
ThreadPool::ThreadPool() : stop(false){
    int size = std::thread::hardware_concurrency() - 2;
    std::cout << "create " << size << " thread\n";
    for (int i = 0; i < size; ++i) {
        threads.emplace_back(std::thread([this]() {
            std::cout << "thread id: " << std::this_thread::get_id() << "\n";
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(my_mutex);
                    cv.wait(lock, [this]() { 
                        return stop || !tasks.empty();  // 为false时等待，为true时继续执行
                        // 任务队列为空时返回true
                    });
                    // （1）stop为true时，短路运算，不会判断tasks.empty()与否
                    if (stop && tasks.empty()) {  // 在这里判断，如果tasks为空，那么退出线程；否则tasks不为空，pop元素不会抛异常
                        return;  // 退出方法执行，结束线程
                    }
                    // （2）stop为false，wait那里就对tasks是否为空进行了判断，只有tasks非空（empty()返回false）时才会退出wait条件等待
                    // 所以下面的pop也是不会报错
                    task = tasks.front();
                    tasks.pop();
                }
                task();
            }
        }
    ));
    }
}
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(my_mutex);
        stop = true;
    }
    cv.notify_all();
    for (std::thread& th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }
}
void ThreadPool::addTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(my_mutex);
        if (stop) {
            throw std::runtime_error("ThreadPool already stop, can't add task any more.");
        }
        tasks.emplace(task);
    }
    cv.notify_one();
}