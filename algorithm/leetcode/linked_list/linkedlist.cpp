#include "linkedlist.h"

#include <iostream>

ListNode* LinkedList::merge(ListNode* left, ListNode* right) { 
    if (!left) return right;
    if (!right) return left;
    if (left->val < right->val) {
        left->next = merge(left->next, right);
        return left;
    } else {
        right->next = merge(left, right->next);
        return right;
    }
}

ListNode* LinkedList::mergeSort(ListNode* head) { 
    // head为空或者只有一个元素，均有序，直接返回
    if (!head || !head->next) return head;
    // 利用快慢指针找到中间节点
    ListNode* slow = head;
    ListNode* fast = head;
    ListNode* prev = nullptr;
    while (fast && fast->next) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    prev->next = nullptr;  // 将链表分成两部分
    // 递归排序
    ListNode* left = mergeSort(head);
    ListNode* right = mergeSort(slow);
    // 合并已排序的两部分
    this->head = merge(left, right); 
    return this->head;
}

LinkedList::LinkedList() : head(nullptr) {}

LinkedList::LinkedList(int arr[], int len) { head = create(arr, len); }

LinkedList::LinkedList(const LinkedList& other) {}

LinkedList::~LinkedList() {}

ListNode* LinkedList::create(int arr[], int len) {
    ListNode* headnode = new ListNode();
    ListNode* p = headnode;
    for (int i = 0; i < len; i++) {
        ListNode* node = new ListNode(arr[i]);
        p->next = node;
        p = p->next;
    }
    head = headnode->next;
    delete headnode;
    return head;
}

ListNode* LinkedList::getHead() const { return head; }

/**
 * 采用归并排序对列表排序
 */
void LinkedList::sort() {
    mergeSort(head);
}

std::ostream& operator<<(std::ostream& cout, const LinkedList& list) {
    using namespace std;
    ListNode* p = list.head;
    while (p != nullptr) {
        cout << p->val;
        if (p->next != nullptr) {
            cout << "->";
        } else {
            cout << endl;
        }
        p = p->next;
    }
    return cout;
}
