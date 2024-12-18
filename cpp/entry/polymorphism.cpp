// #include <iostream>
// #include <cstdio>
// using namespace std;

// class A {
//     private:
//         int i;
//     public:
//         A() : i(10) {}
//         virtual void f(){cout << "A::f(), i = " << i << endl;}
//         virtual void f2(){cout << "A::f2(), i = " << i << endl;}
// };


// int main() {
//     A a;
//     // 1、without virtul
//     cout << sizeof(a) << endl; // 4 bytes
//     // 2、with virtual (maybe 8字节vtable ptr, 4字节成员变量, 为了凑8的整数倍, 多出来4字节)
//     // cout << sizeof(a) << endl; // 16 bytes
//     size_t* base = (size_t*)&a;
//     int t = *(base + 1);
//     cout << "value of member i: " << t << endl; // 10
//     cout << "value of vtable ptr: " << hex << (size_t*)*base << endl;

//     // 获取虚函数表中的虚函数地址
//     size_t* vtablePtr = (size_t*)*base;
//     cout << "address of A::f() in vtable: " << hex << vtablePtr[0] << endl;  // 第一个虚函数 f 的地址
//     cout << "address of A::f2() in vtable: " << hex << vtablePtr[1] << endl;  // 第二个虚函数 f2 的地址

//     void (A::*func_ptr)() = &A::f;
//     void (A::*func_ptr2)() = &A::f2;
//     cout << "address of A::f(): " << hex << (size_t)(void*)&func_ptr << endl;  // 输出 func_ptr 的地址
//     cout << "address of A::f2(): " << hex << (size_t)(void*)&func_ptr2 << endl;  // 输出 func_ptr2 的地址
//     cout << "==========================================" << endl;

//     // A* a_ptr = new A();
//     // // 获取虚函数表指针（即对象的第一个成员指针）
//     // size_t* vtablePtr = *(size_t**)a_ptr;
//     // printf("address of vt_ptr: %p\n", vtablePtr);
//     // printf("value of vt_ptr: %p\n", *vtablePtr);
//     // // 获取虚函数表中存储的第一个虚函数地址
//     // void (*f_ptr)() = (void(*)())vtablePtr[0];
//     // void (*f_ptr2)() = (void(*)())vtablePtr[1];
//     // // f_ptr();
//     // printf("address of A::f(): %p\n", f_ptr);
//     // printf("address of A::f2(): %p\n", f_ptr2);
//     return 0;
// }

#include <iostream>
using namespace std;

class A {
public:
    int i = 10;
    virtual void f() {
        cout << "A::f()" << endl;
    }
    virtual void f2() {
        cout << "A::f2()" << endl;
    }
};

int main() {
    A a;
    cout << sizeof(a) << endl; // 16 bytes, with virtual functions

    size_t* base = (size_t*)&a;
    cout << "vtable ptr address: " << hex << *base << endl; // 虚函数表的地址，55ca91081d48

    int t = *(base + 1);  // 获取成员变量i的值
    cout << "value of member i: " << t << endl; // 输出成员变量 i 的值

    // 获取虚函数表中的虚函数地址
    size_t* vtablePtr = (size_t*)*base;
    cout << "address of A::f() in vtable: " << hex << &vtablePtr[0] << endl;  // 第一个虚函数 f 在table中的地址，0x55ca91081d48
    cout << "address of A::f2() in vtable: " << hex << &vtablePtr[1] << endl;  // 第二个虚函数 f2 在table中的地址，0x55ca91081d50（刚好是一个2个字节的偏移）

    cout << "address of A::f(): " << hex << vtablePtr[0] << endl;  // 第一个虚函数 f 的地址
    cout << "address of A::f2(): " << hex << vtablePtr[1] << endl;

    void (A::*func_ptr)() = &A::f;
    void (A::*func_ptr2)() = &A::f2;
    cout << "address of A::f(): " << hex << (size_t)(void*)&func_ptr << endl;  // 输出 func_ptr 的地址
    cout << "address of A::f2(): " << hex << (size_t)(void*)&func_ptr2 << endl;  // 输出 func_ptr2 的地址

    return 0;
}
