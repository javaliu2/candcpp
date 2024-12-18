#ifndef VECTOR_H
#define VECTOR_H
#include <cstddef>
#include <iostream>
using namespace std;
// 支持任意类型的列表类模版，底层采用数组实现
// 类模板（declaration）
template <class T>
class Vector {
    private:
        size_t capacity;
        size_t size;
        T *data;
    public:
        Vector();
        Vector(size_t capacity);
        Vector(const Vector<T>& v);
        ~Vector();
        // 函数模板（declaration）
        T& operator[](size_t index);
        void append(T value);
        void sort();  // in-place
        template <class U>
        friend ostream& operator<<(ostream& cout, const Vector<U>& v);
        size_t getSize() const;
        size_t getCapacity() const;
};
#endif

// 模板函数（definition）
template <class T>
Vector<T>::Vector() : capacity(4), size(0) {
    data = new T[capacity];
}

template <class T>
Vector<T>::Vector(size_t capacity) : capacity(capacity), size(0) {
    data = new T[capacity];
}

template <class T>
Vector<T>::Vector(const Vector<T>& v) {
    capacity = v.capacity;
    size = v.size;
    data = new T[capacity];
    for (int i = 0; i < size; i++) {
        data[i] = v.data[i];
    }
}

template <class T>
Vector<T>::~Vector() {
    delete[] data;
}

template <class T>
T& Vector<T>::operator[](size_t index) {
    if (index >= size) {
        return nullptr;
    }
    return data[index];
}

template <class T>
void Vector<T>::append(T value) {
    if (size + 1 >= capacity) {
        capacity *= 2;
        T *t = new T[capacity];
        for (int i = 0; i < size; i++) {
            t[i] = data[i];
        }
        delete[] data;
        data = t;
    }
    data[size] = value;
    size++;
}

// T类型对象需要实现自己的operator=
// global scope
template <class T>
void swap_(T& a, T& b) {
    T temp = a;  // 拷贝构造
    a = b;  // 赋值
    b = temp;  // 赋值
}

template <class T>
void Vector<T>::sort() {
    // 冒泡排序，模板函数版本
    for (int i = 0; i < size - 1; i++) {
        for (int j = size - 1; j > i; j--) {
            if (data[j] < data[j - 1]) {
                swap_(data[j], data[j - 1]);
            }
        }
    }
}

template <class T> 
size_t Vector<T>::getSize() const {
    return size;
}

template <class T> 
size_t Vector<T>::getCapacity() const {
    return capacity;
}



template <class U>
ostream& operator<<(ostream& os, const Vector<U>& v) {
    os << "[";
    for (size_t i = 0; i < v.size; i++) {
        os << v.data[i];
        if (i + 1 < v.size) {
            os << ", ";
        }
    }
    os << "]\n";
    return os;
}


