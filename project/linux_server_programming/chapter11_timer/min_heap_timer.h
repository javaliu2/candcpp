/**
 * 代码清单11-6 时间堆
 * 我感觉，这个代码鲁棒性不是很好，反正这个思想领会到就可以，使用的话可以直接使用STL
 */
#ifndef MIN_HEAP
#define MIN_HEAP
#include <iostream>
#include <netinet/in.h>
#include <time.h>
using std::exception;
#define BUFFER_SIZE 64
class heap_timer;
struct client_data {
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    heap_timer* timer;
};
/* 定时器类 */
class heap_timer {
    public:
        heap_timer(int delay) {
            expire = time(NULL) + delay;
        }
    public:
        time_t expire;  // 定时器生效的绝对时间
        void (*cb_func)(client_data*);
        client_data* user_data;
};
/* 时间堆类 */
class time_heap {
    public:
        /* 构造函数之一，初始化一个大小为cap的空堆 */
        time_heap(int cap) : capacity(cap), cur_size(0) {
            array = new heap_timer*[capacity];
            if (!array) {
                throw std::exception();
            }
            for (int i = 0; i < capacity; ++i) {
                array[i] = nullptr;
            }
        }
        /* 构造函数之二，用已有数组来初始化堆 */
        time_heap(heap_timer** init_array, int size, int capacity) : cur_size(size), capacity(capacity) {
            if (capacity < size) {
                throw std::exception();
            }
            array = new heap_timer*[capacity];
            if (!array) {
                throw std::exception();
            }
            for (int i = 0; i < capacity; ++i) {
                array[i] = nullptr;
            }
            if (size != 0) {
                for (int i = 0; i < size; ++i) {
                    array[i] = init_array[i];
                }
                /* 对数组中非叶子节点执行下滤操作 */
                for (int i = (cur_size - 1) / 2; i >= 0; --i) {
                    percolate_down(i);
                }
            }
        }
        ~time_heap() {
            for (int i = 0; i < cur_size; ++i) {
                delete array[i];
            }
            delete[] array;
        }
        void add_timer(heap_timer* timer) {
            if (!timer) {
                return;
            }
            if (cur_size >= capacity) {
                resize();
            }
            int hole = cur_size++;  // 新建空穴的位置
            int parent = 0;
            /* 对从空穴到根节点的路径上的所有节点执行上滤操作 */
            for (; hole > 0; hole = parent) {
                parent = (hole - 1) / 2;
                if (array[parent]->expire <= timer->expire) {
                    break;
                }
                array[hole] = array[parent];
            }
            array[hole] = timer;
        }
        void del_timer(heap_timer* timer) {
            if (!timer) {
                return;
            }
            /* 仅仅将目标定时器的回调函数设置为空，即所谓的延迟销毁。
            这将节省真正删除该定时器造成的开销，但这样做容易使堆数组膨胀 */
            timer->cb_func = nullptr;
        }
        heap_timer* top() const {
            if (empty()) {
                return nullptr;
            }
            return array[0];
        }
        void pop_timer() {
            if (empty()) {
                return;
            }
            if (array[0]) {
                delete array[0];
                array[0] = array[--cur_size];
                percolate_down(0);
            }
        }
        void tick() {
            heap_timer* tmp = array[0];
            time_t cur = time(NULL);
            while (!empty()) {
                if (!tmp) {
                    break;
                }
                if (tmp->expire > cur) {
                    break;
                }
                if (array[0]->cb_func) {
                    array[0]->cb_func(array[0]->user_data);
                }
                pop_timer();
                tmp = array[0];
            }
        }
        bool empty() const {
            return cur_size == 0;
        }
    private:
        /* 小根堆的下滤操作，它确保堆数组中索引为hole的节点作为根的树拥有小根堆性质 */
        void percolate_down(int hole) {
            heap_timer* temp = array[hole];
            int child = 0;
            for (; hole * 2 + 1 <= cur_size - 1; hole = child) {
                child = hole * 2 + 1;
                if (child < cur_size - 1 && array[child + 1]->expire < array[child]->expire) {
                    ++child;
                }
                if (array[child]->expire < temp->expire) {
                    array[hole] = array[child];
                } else {
                    break;
                }
            }
            array[hole] = temp;
        }
        void resize() {
            heap_timer** temp = new heap_timer*[2*capacity];
            if (!temp) {
                throw std::exception();
            }
            for(int i = 0; i < 2*capacity; ++i) {
                temp[i] = nullptr;
            }
            capacity *= 2;
            for (int i = 0; i < cur_size; ++i) {
                temp[i] = array[i];
            }
            delete[] array;
            array = temp;
        }
        heap_timer** array;
        int capacity;
        int cur_size; 
};
#endif