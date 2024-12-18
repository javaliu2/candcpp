#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "listnode.h"
#include <iostream>
class LinkedList {
    private:
        ListNode* head;
        ListNode* merge(ListNode* left, ListNode* right);
        ListNode* mergeSort(ListNode* head);
    public:
        LinkedList();
        LinkedList(int arr[], int len);
        LinkedList(const LinkedList& other);
        ~LinkedList();
        ListNode* create(int arr[], int len);
        ListNode* getHead() const;
        friend std::ostream& operator<<(std::ostream& o, const LinkedList& list);
        void sort();
};

#endif
