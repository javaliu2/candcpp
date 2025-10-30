#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int x = 0;
mutex my_lock;
void func() {
    for (int i = 0; i < 10000; ++i) {
        my_lock.lock();
        ++x;
        my_lock.unlock();
    }
}

void func2() {
    for (int i = 0; i < 10000; ++i) {
        /**
         * A simple scoped lock type.
         A lock_guard controls mutex ownership within a scope, 
         releasing ownership in the destructor.
         一个简单的范围锁类型。lock_guard在一个范围内控制mutex的所有权，在析构函数中释放所有权
         */
        lock_guard<mutex> lg(my_lock);
        ++x;
    }
}
timed_mutex t_mutex;
void func3() {
    for (int i = 0; i < 10000; ++i) {
        /**
         * A movable scoped lock type.
         * A unique_lock controls mutex ownership within a scope. 
         * Ownership of the mutex can be delayed until after construction and 
         * can be transferred to another unique_lock by move construction or move assignment. 
         * If a mutex lock is owned when the destructor runs ownership will be released.
         * 一个可移动的范围锁类型。
         */
        unique_lock<timed_mutex> ul(t_mutex, chrono::seconds(3));
        ++x;
    }
}

int main() {
    cout << "before tasks execute, x: " << x << endl;
    thread t1(func3);
    thread t2(func3);
    t1.join();
    t2.join();
    cout << "before tasks executed, x: " << x << endl;
    return 0;
}