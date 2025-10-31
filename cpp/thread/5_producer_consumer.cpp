#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <random>
#include <unordered_map>

// 全局map，统计每一个数字的出现次数
std::unordered_map<int, int> g_counter;
std::mutex g_counter_mutex;

class Slot {
private:
    int capacity;
    int* slots;
    int size;
    std::mutex mutex;
    std::condition_variable slot_empty;
    std::condition_variable slot_full;
public:
    Slot() : capacity(10), slots(new int[capacity]), size(0) {}
    Slot(int capacity) : capacity(capacity), slots(new int[capacity]), size(0) {}
    ~Slot() {
        delete[] slots;
    }
    void produce(int task) {
        std::unique_lock<std::mutex> lock(mutex);
        // 槽位已满，等待消费者消费，有空槽位再提交新任务
        // while (size >= capacity) {
        //     slot_full.wait(lock);
        // }
        slot_full.wait(lock, [&]{ return size < capacity; });
        // 槽位未满，提交新任务
        slots[size++] = task;
        {
            std::lock_guard<std::mutex> guard(g_counter_mutex);
            g_counter[task]++;
        }
        std::cout << "thread_id: " << std::this_thread::get_id();
        std::cout << " produce task: " << task << std::endl;
        slot_empty.notify_one();
    }
    void consume() {
        std::unique_lock<std::mutex> lock(mutex);
        // 槽位为空，阻塞等待生产者提交任务
        // while (size == 0) {
        //     slot_empty.wait(lock);
        // }
        // 带谓词形式的wait，等价于上面的while(condition)，但是推荐使用下面的方式
        slot_empty.wait(lock, [&]{ return size > 0; });  // 后者为false时等待，为true时继续执行
        // 槽位不为空，消费任务
        int task = slots[--size];
        {
            std::lock_guard<std::mutex> guard(g_counter_mutex);
            if (--g_counter[task] == 0) {
                g_counter.erase(task);
            }
        }
        std::cout << "thread_id: " << std::this_thread::get_id();
        std::cout << " consume task: " << task << std::endl;
        slot_full.notify_one();
    }
};

int min = 0,max = 100;
std::random_device seed;//硬件生成随机数种子
std::ranlux48 engine(seed());//利用种子生成随机数引擎
std::uniform_int_distribution<> distrib(min, max);

void Producer(Slot& slot, int n) {
    for (int i = 0; i < n; ++i) {
        slot.produce(distrib(engine));
    }
}

void Consumer(Slot& slot, int n) {
    for (int i = 0; i < n; ++i) {
        slot.consume();
    }
}
int main() {
    Slot slot;
    std::thread producer(Producer, std::ref(slot), 4);
    std::thread producer2(Producer, std::ref(slot), 5);
    std::thread producer3(Producer, std::ref(slot), 7);

    std::thread consumer(Consumer, std::ref(slot), 4);
    std::thread consumer2(Consumer, std::ref(slot), 3);
    std::thread consumer3(Consumer, std::ref(slot), 4);
    std::thread consumer4(Consumer, std::ref(slot), 5);

    producer.join();
    producer2.join();
    producer3.join();
    consumer.join();
    consumer2.join();
    consumer3.join();
    consumer4.join();

    if (g_counter.empty()) {
        std::cout << "(empty map)" << std::endl;
    } else {
        for (auto& [num, count] : g_counter) {
            std::cout << "number: " << num << "; count: " << count << std::endl;
        }
    }
    return 0;
}