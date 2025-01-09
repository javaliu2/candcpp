#include "min_heap_array_impl_spec.h"
#include <vector>
void ArrayMinHeap::heapifyUp(int i) {
    /**
     * 判断heap[i]与其父节点值f_v的大小
     * case 1: 小于f_v，与父节点交换，递归调整父节点与其父节点的关系
     * case 2: 大于等于f_v，不用进行调整，函数返回
     * 
     */
    int father_idx = (i - 1) / 2;
    if (father_idx < 0) {
        return;
    }
    if (heap[i] < heap[father_idx]) {
        myswap(i, father_idx);
        heapifyUp(father_idx);
    }
}

/**
 * 对索引 i 确定的二叉树进行下调操作
 */
void ArrayMinHeap::heapifyDown(int i) {
    // 一定存在左节点
    int end = heap.size() - 1;
    int left_idx = 2 * i + 1, right_idx = 2 * i + 2, tmp_idx = left_idx;
    if (left_idx > end) {
        return;
    }
    if (right_idx <= end) {
        if (heap[right_idx] < heap[left_idx]) {
            tmp_idx = right_idx;
        }
    }
    if (heap[tmp_idx] < heap[i]) {
        myswap(i, tmp_idx);
        heapifyDown(tmp_idx);
    }
}

void ArrayMinHeap::myswap(int i, int j) {
    int t = heap[i];
    heap[i] = heap[j];
    heap[j] = t;
}

ArrayMinHeap::ArrayMinHeap() {}

ArrayMinHeap::ArrayMinHeap(std::vector<int>& data) : heap(data) {
    /**
     * 对乱序的数据data进行建堆，只需要对所有非叶子结点进行调整，
     * 那么调整后的完全二叉树就满足小根堆的性质
     * 最后一个节点的索引为data.len-1，记为end,
     * 那么它的父节点索引为floor((end-1)/2)
     * 从floor((end-1)/2)值确定的树开始调整，直至索引为0的树
     * 需要注意，当子节点与父节点进行交换之后，需要对被交换的子树进行调整
     */
    int end = heap.size() - 1;
    for (int i = (end - 1) / 2; i >= 0; i--) {
        heapifyDown(i);
    }
}

void ArrayMinHeap::add(int value) {
    /**
     * 堆已经有序，将新值置于 end+1 的位置，调用 heapifyUp 对树进行调整
     * 
     */
    // heap[end + 1] = value;  // Segmentation fault (core dumped), 
    // 因为此时没有元素，operator[]是访问已有的元素，而不是添加元素
    heap.push_back(value);
    int end = heap.size() - 1;
    heapifyUp(end);
}

int ArrayMinHeap::remove() {
    /**
     * 返回最小值
     * 将最后一个节点置于根节点，调用heapifyDown调整整棵树
     * 
     */
    int tmp = heap[0];
    int end = heap.size() - 1;
    heap[0] = heap[end];
    // delete heap[end]
    heap.pop_back(); // 该方法是否会调用对象的析构函数
    heapifyDown(0);
    return tmp;
}

int ArrayMinHeap::peek() const {
    return heap[0];
}
bool ArrayMinHeap::isEmpty() const {
    return heap.empty();
}
int ArrayMinHeap::size() const {
    return heap.size();
}

/**
 * 辅助函数，广度优先遍历查看树
 */
void ArrayMinHeap::bfs() const {
    int end = heap.size() - 1;
    std::queue<int> q;
    q.push(0);  // 根节点的索引
    while (!q.empty()) {
        int n = q.size();
        while (n--) {
            int node_idx = q.front();
            q.pop();
            std::cout << heap[node_idx] << " ";
            if (node_idx * 2 + 1 <= end) {
                q.push(node_idx * 2 + 1);
            }
            if (node_idx * 2 + 2 <= end) {
                q.push(node_idx * 2 + 2);
            }
        }
        std::cout << std::endl;
    }
}

void ArrayMinHeap::print() {
    while (!isEmpty()) {
        int top_ele = remove();
        std::cout << top_ele << " ";
    }
    std::cout << std::endl;
}
