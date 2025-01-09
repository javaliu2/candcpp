#ifndef MIN_HEAP_H
#define MIN_HEAP_H
/**
 * 小根堆 specification
 * 小根堆是一棵完全二叉树，非叶子结点的值 不大于 其左孩子和右孩子的值
 */
class MinHeap {
    public:
        virtual ~MinHeap() = default;
        virtual void add(int value) = 0;
        virtual void remove() = 0;
        virtual int peek() const = 0;
        virtual bool isEmpty() const = 0;
        virtual int size() const = 0;
}; 
#endif