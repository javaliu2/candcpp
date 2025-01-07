/**
 * 代码清单11-2 升序定时器链表
 */
#ifndef LIST_TIMER
#define LIST_TIMER
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#define BUFFER_SIZE 64
class util_timer; // 前向声明, declaration
/* 用户数据结构：客户端socket地址，socket fd，读缓存和定时器 */
struct client_data {
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    util_timer* timer;
};
/* 定时器类 */
class util_timer {
    public:
        util_timer() : prev(nullptr), next(nullptr), user_data(nullptr){}
        util_timer* prev;
        util_timer* next;
        client_data* user_data;
        time_t expire;  // 任务的超时时间，这里使用绝对时间
        void(*cb_func)(client_data*);  // 任务回调函数
};
/* 定时器链表。升序、双向链表且带有头结点和尾节点 */
class sorted_timer_list {
    private:
        util_timer* head;
        util_timer* tail;
    public:
        sorted_timer_list() : head(nullptr), tail(nullptr){}
        /* 链表被销毁时，删除其中所有的定时器 */
        ~sorted_timer_list() {
            util_timer* p = head;
            while (p) {
                head = p->next;
                delete p;
                p = head;
            }
        }
        /* 将目标定时器timer添加到链表中 */
        void add_timer(util_timer* timer) {
            if (!timer) {
                return;
            }
            if (!head) {
                head = tail = timer;
                return;
            }
            if (timer->expire < head->expire) {
                timer->next = head;
                head->prev = timer;
                head = timer;
                return;
            }
            add_timer(timer, head);
        }
        /* 当某个定时任务发生变化时，调整对应的定时器在链表中的位置。
         * 该函数只考虑调整的定时器的超时时间延长的情况，即该定时器需要往链表的尾部移动
        */
       void adjust_timer(util_timer* timer) {
            if (!timer) {
                return;
            }
            util_timer* tmp = timer->next;
            if (!tmp || timer->expire < tmp->expire) {
                return;
            }
            if (timer == head) {
                head = head->next;
                head->prev = nullptr;
                timer->next = nullptr;
                add_timer(timer, head);
            } else {
                timer->prev->next = timer->next;
                timer->next->prev = timer->prev;
                add_timer(timer, timer->next);
            }
       }
       void del_timer(util_timer* timer){
            if(!timer) {
                return;
            }
            if (timer == head && timer == tail) {
                delete timer;
                head = nullptr;
                tail = nullptr;
                return;
            }
            if (timer == head) {
                head = head->next;
                head->prev = nullptr;
                delete timer;
                return;
            }
            if (timer == tail) {
                tail = tail->prev;
                tail->next = nullptr;
                delete timer;
                return;
            }
            timer->prev->next = timer->next;
            timer->next->prev = timer->prev;
            delete timer;
       }
       /**
        * SIGALRM信号每次被触发就在其信号处理函数（如果使用统一信号源，则是主函数）
        * 中执行一次tick函数，以处理链表上到期的任务
        */
       void tick() {
            if (!head) {
                return;
            }
            printf("timer tick\n");
            time_t cur = time(NULL);  // 获得系统当前时间
            util_timer* tmp = head;
            while (tmp) {
                if (cur < tmp->expire) {
                    break;
                }
                /* 调用定时器的回调函数，以执行定时任务 */
                tmp->cb_func(tmp->user_data);
                head = tmp->next;
                if (head) {
                    head->prev = nullptr;
                }
                delete tmp;
                tmp = head;
            }
       }
    private:
        /* 将timer添加到以节点head之后的部分链表中 */
        void add_timer(util_timer* timer, util_timer* head) {
            // timer->expire 一定>= head->expire
            util_timer* prev = head, *tmp = head->next;
            while (tmp != nullptr && timer->expire > tmp->expire) {
                prev = tmp;
                tmp = tmp->next;
            }
            prev->next = timer;
            timer->prev = prev;
            if (tmp) {
                timer->next = tmp;
                tmp->prev = timer;
            } else {
                timer->next = nullptr;
                tail = timer;
            }
        }

};
#endif