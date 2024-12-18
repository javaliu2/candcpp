#include "linkedlist.h"
#include <iostream>

void mySwap(int arr[], int i, int j) {
    int t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}
int partition(int arr[], int l, int r) {
    int pivot_idx = l + (r - l) / 2;
    int pivot = arr[pivot_idx];
    int storeIdx = l;
    mySwap(arr, pivot_idx, r);
    // for (int i = 0; i < r; i++) {  // attention！不是0啊，参数是变化的
    for (int i = l; i < r; i++) {
        if (arr[i] < pivot) {
            mySwap(arr, i, storeIdx);
            storeIdx++;
        }
    }
    mySwap(arr, r, storeIdx);
    return storeIdx;
}
void quick_sort(int arr[], int l, int r) {
    if (l < r) {
        int mid = partition(arr, l, r);
        quick_sort(arr, l, mid - 1);
        quick_sort(arr, mid + 1, r);
    }
}

static const int LEN = 5 * 10000 + 1;
static int arr[LEN];

ListNode* sortList(ListNode* head) {
    // 1、遍历链表，将value存到数组arr中
    ListNode* p = head;
    int idx = 0;
    while (p != nullptr) {
        arr[idx++] = p->val;
        p = p->next;
    }
    // 2、排序
    quick_sort(arr, 0, idx - 1);
    // 3、将有序数据放入链表
    p = head;
    for (int i = 0; i < idx; i++) {
        p->val = arr[i];
        p = p->next;
    }
    return head;
}

int main() {
    int arr[] = {4, 2, 1, 3};
    int arr2[] = {4, 2, 1, 3, 6};
    LinkedList l1(arr, 4);
    LinkedList l2(arr2, 5);
    ListNode* head = l1.getHead();
    std::cout << l1;
    // head = sortList(head);
    l1.sort();
    // std::cout << l1 << l2;
    std::cout << l1;
    return 0;
}