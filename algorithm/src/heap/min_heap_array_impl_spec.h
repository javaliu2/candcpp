#ifndef MIN_HEAP_ARRAY_IMPL_SPEC_H
#define MIN_HEAP_ARRAY_IMPL_SPEC_H
#include "min_heap_spec.h"
#include <vector>

/**
 * 这里采用数组表示完全二叉树.
 * 索引为i的节点，其左孩子节点的索引: 2*i+1; 右孩子节点的索引: 2*i+2; 父节点索引: floor( (i-1)/2 )
 * 
 * 
*/
class ArrayMinHeap : public MinHeap {
    private:
        std::vector<int> heap;
        void heapifyUp();
        void heapifyDown();
    public:
        ArrayMinHeap();
        ArrayMinHeap(std::vector<int>& data);
        virtual void add(int value) override;
        virtual void remove() override;
        virtual int peek() const override;
        virtual bool isEmpty() const override;
        virtual int size() const override;
};
#endif