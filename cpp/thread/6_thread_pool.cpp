#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
/**
 * mutex是锁本身（共享），unique_lock是锁的管理工具（局部）
 */
class ThreadPool {
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable task_condition;
    std::condition_variable enqueue_condition;
    bool stop;
    const size_t max_queue_size = 100;
public:
    ThreadPool(int numThreads, size_t maxQueueSize=100) : stop(false), max_queue_size(maxQueueSize) {
        if (numThreads <= 0) {
            numThreads = 1;
        }
        if (max_queue_size <= 0) {
            throw std::invalid_argument("max_queue_size must be > 0.");
        }
        for (int i = 0; i < numThreads; ++i) {
            // 使用匿名函数，构建线程所需func
            // 捕获this指针，以访问ThreaPool的成员变量
            threads.emplace_back([this]{
                while (true) {
                    std::unique_lock<std::mutex> lock(mutex);
                    task_condition.wait(lock, [this]{ return stop || !tasks.empty(); });  // 自旋等待条件满足
                    if (stop && tasks.empty()) {
                        std::cout << "thread " << std::this_thread::get_id() << " return" << std::endl;
                        return;
                    }
                    // tasks.front()返回std::function<void()>类型的左值引用
                    // move(): 将其标记为右值，告诉编译器，该值可以被窃取
                    std::function<void()> task(std::move(tasks.front()));  // 使用移动构造函数
                    tasks.pop();  // 队列空出来一个位置
                    enqueue_condition.notify_one();  // 通知入队但是由于queue满而阻塞的线程，如果有的话
                    lock.unlock();
                    try {
                        task();
                    } catch (const std::exception& e) {
                        std::cerr << "task execution failed: " << e.what() << std::endl;
                    }
                }
            });
        }
    }
    ~ThreadPool() {
        std::cout << "~ThreadPool()" << std::endl;
        {
            std::unique_lock<std::mutex> lock(mutex);
            stop = true;  // 对共享变量的修改必须加锁
        }
        // 唤醒所有线程
        // 1、task_conditon唤醒所有工作线程（处理任务，消费者），让他们检查退出条件
        // 2、enqueue_condition唤醒所有等待任务入队的线程（生产任务，生产者），让他们退出等待
        task_condition.notify_all();
        enqueue_condition.notify_all();
        // 等待所有工作线程退出
        for (std::thread& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
    template<typename F, typename... Args>
    void enqueue(F&& f, Args&&... args) {
        std::function<void()> task(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        {
            std::unique_lock<std::mutex> lock(mutex);
            // 调用enqueue的线程等待条件满足
            enqueue_condition.wait(lock, [this] {return stop || tasks.size() <= max_queue_size; });
            if (stop) {
                throw std::runtime_error("ThreadPool is stopped, can't enqueue new task.");
            }
            tasks.emplace(std::move(task));
        }
        task_condition.notify_one();  // 通知工作线程处理任务
    }
};


int main(){
    ThreadPool pool(4, 8);  // 4个线程，8个容量
    for (int i = 0; i < 15; ++i) {  // 15个任务
        pool.enqueue([i]{
            std::cout << "Task " << i << " is running in thread " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Task " << i << " is done" << std::endl;
        });
    }
    int a = 3;
    pool.enqueue([&]{
        ++a;
        std::cout << "Task ++a" << " is running in thread " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Task ++a finish, a: " << a << std::endl;
    });
    // std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "in main(), a: " << a << std::endl;
    return 0;
}