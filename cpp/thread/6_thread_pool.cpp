#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>

class ThreadPool {
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable condition;
    bool stop;
public:
    ThreadPool(int numThreads) : stop(false) {
        for (int i = 0; i < numThreads; ++i) {
            // 使用匿名函数，构建线程所需func
            // 捕获this指针，以访问ThreaPool的成员变量
            threads.emplace_back([this]{
                while (true) {
                    std::unique_lock<std::mutex> lock(mutex);
                    condition.wait(lock, [this]{ return stop || !tasks.empty(); });
                    if (stop && tasks.empty()) {
                        return;
                    }
                    // tasks.front()返回std::function<void()>类型的左值引用
                    // move(): 将其标记为右值，告诉编译器，该值可以被窃取
                    std::function<void()> task(std::move(tasks.front()));  // 使用移动构造函数
                    tasks.pop();
                    lock.unlock();
                    task();
                }
            });
        }
    }
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& thread : threads) {
            thread.join();
        }
    }
    template<typename F, typename... Args>
    void enqueue(F&& f, Args&&... args) {
        std::function<void()> task(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        {
            std::unique_lock<std::mutex> lock(mutex);
            tasks.emplace(std::move(task));
        }
        condition.notify_one();
    }
};


int main(){
    ThreadPool pool(4);
    for (int i = 0; i < 8; ++i) {
        pool.enqueue([i]{
            std::cout << "Task " << i << " is running in thread " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Task " << i << " is done" << std::endl;
        });
    }
    return 0;
}