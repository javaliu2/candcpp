#include "listnode.h"
#include <stack>

class Solution {
public:
    ListNode* head = nullptr;
    /**
     * 递归，隐式调用栈
     */
    ListNode* reverseList(ListNode* head) {
        if (head == nullptr) {
            return head;
        }
        // attention, 还是循环链表的问题
        ListNode* tail = f(head);
        tail->next = nullptr;

        return this->head;
    }
    ListNode* f(ListNode* node) {
        if (node->next == nullptr) {
            this->head = node;
            return node;
        }
        ListNode* tail = f(node->next);
        tail->next = node;
        return node;
    }

    /**
     * 显式使用栈
     */
    ListNode* reverseList_iteration(ListNode* head) {
        // 注意测试用例的范围
        if (head == nullptr) {
            return head;
        }
        std::stack<ListNode*> s;
        // 1、遍历链表，逐元素入栈
        ListNode* p = head;
        while (p->next) {
            s.push(p);
            p= p->next;
        }
        // 遍历结束，p指向最后一个元素
        ListNode* head = p;
        while (!s.empty()) {
            ListNode* t = s.top();
            p->next = t;
            p = t;
            s.pop();
        }
        p->next = nullptr;
        return head;
    }
};