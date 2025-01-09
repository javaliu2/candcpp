#include <iostream>
#include <vector>
#include <random>
#include "min_heap_array_impl_spec.h"
using namespace std;

int main() {
    vector<int> v;
    cout << v.size() <<endl;
    v.push_back(24);
    v.push_back(31);
    v.push_back(32);
    v.push_back(26);
    v.push_back(65);
    v.push_back(21);
    
    // ArrayMinHeap min_heap(v);
    // min_heap.bfs();
    // vector<int> v2(v);
    // for (int e : v2) {
    //     cout << e << " ";
    // }
    int cnt = 10;
    while (cnt--) {
        int number = random() % 40;
        cout << "number: " << number << endl;
        ArrayMinHeap min_heap;
        while (number--) {
            int element = random() % 40 - 20;
            min_heap.add(element);
        }
        // min_heap.bfs();
        min_heap.print();
    }
    return 0;
}