#include "useless.h"
#include <iostream>
using namespace std;
int Useless::ct = 0;

Useless::Useless() {
    ++ct;
    n = 0;
    pc = nullptr;
    cout << "default constructor called; number of objects: " << ct << endl;
    ShowObject();
}

Useless::Useless(int k) : n(k) {
    ++ct;
    pc = new char[n];
    cout << "int constructor called; number of objects: " << ct << endl;
    ShowObject();
}

Useless::Useless(int k, char ch) : n(k) {
    ++ct;
    pc = new char[n];
    for (int i = 0; i < n; ++i) {
        pc[i] = ch;
    }
    cout << "int,char constructor called; number of objects: " << ct << endl;
    ShowObject();
}

Useless::Useless(const Useless& f) : n(f.n) {
    ++ct;
    cout << "copy constructor called; number of objects: " << ct << endl;
    pc = new char[n];
    for (int i = 0; i < n; ++i) {
        pc[i] = f.pc[i];  // 在成员函数内，可以访问同类对象的private变量
    }
    ShowObject();
}

Useless::Useless(Useless&& f) : n(f.n) {
    ++ct;
    cout << "move constructor called; number of objects: " << ct << endl;
    pc = f.pc;
    f.pc = nullptr;
    f.n = 0;
    ShowObject();
}

Useless::~Useless() {
    cout << "deconstrucotr called; objects left: " << --ct << endl;
    ShowObject();
    delete[] pc;
}

Useless Useless::operator+(const Useless& f) const{
    cout << "enter operator+()" << endl;
    Useless t = Useless(n + f.n);
    for (int i = 0; i < n; ++i) {
        t.pc[i] = pc[i];
    }
    for (int i = n; i < t.n; ++i) {
        t.pc[i] = f.pc[i - n];
    }
    cout << "leaving operator+()" << endl;
    return t;
}

void Useless::ShowData() const {
    if (n == 0) {
        cout << "empty object" << endl;
    } else {
        for (int i = 0; i < n; ++i) {
            cout << pc[i];
        }
        cout << endl;
    }
}

void Useless::ShowObject() const {
    cout << "Number of elements: " << n;
    cout << "; Data address: " << (void*)pc << endl;
}

Useless& Useless::operator=(const Useless& f) {
    if (this == &f) {
        return *this;
    }
    delete[] pc;
    n = f.n;
    pc = new char[n];
    for (int i = 0; i < n; ++i) {
        pc[i] = f.pc[i];
    }
    return *this;
}
// 对f进行修改，所以不能是const
Useless& Useless::operator=(Useless&& f) {
    if (this == &f) {
        return *this;
    }
    delete[] pc;
    n = f.n;
    pc = f.pc;
    f.n = 0;
    f.pc = nullptr;
    return *this;
}