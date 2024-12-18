#include <iostream>
#include <stack>

#include "linkedlist.h"
#include "listnode.h"

using namespace std;

void reorderList(ListNode* head) {
    std::stack<ListNode*> s;
    int num = 0;  // 记录元素个数
    ListNode *p = head, *q, *next;
    while (p) {
        s.push(p);
        p = p->next;
        num++;
    }
    int cnt = num / 2;
    p = head;
    while (cnt--) {
        q = s.top();  // peek, 而不移除栈顶元素
        s.pop();  // 返回void,只是执行pop操作
        next = p->next;
        p->next = q;                    // 记录当前新链表末尾节点
        if (next != q) q->next = next;  // 顺序遍历
        p = next;
    }
    p->next = nullptr;  // 就差这一行
    // 以l1->l2->l3->l4为例：
    // 当l2->next = l3后，l3现在是结尾节点,l3->next应该为null，但是原始链表中l3->next==l4
    // 这就形成循环链表了，绝对是错误的，因此需要显式指定末尾节点的next为null
}

int main() {
    const int LEN = 5;
    int arr[LEN] = {1, 2, 3, 4, 5};
    LinkedList list(arr, LEN);
    cout << list;
    ListNode* head = list.getHead();
    reorderList(head);
    cout << list;
    return 0;
}