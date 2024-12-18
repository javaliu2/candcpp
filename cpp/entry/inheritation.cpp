#include "animal.h"
#include "cat.h"
#include <iostream>
#include <cstdio>

using namespace std;

int main(){

    Animal a1("Lion", 5);
    Cat cat("xiaomaomi", 3, 44);
    Animal& a = cat;
    a.say();
    // char* p = (char*)&a1;
    // printf("%p\n", p);
    // printf("%s\n", p);
    // a1.say();

    // Animal a2 = a1;  // 调用拷贝构造函数
    // a2.say();

    // Animal a3;
    // a3 = a1;  // 调用赋值运算符
    // a3.say();
    // Cat cat("xiaomaomi", 10, 666);
    // cat.say();
    // char * p = (char*)&cat;
    // cout << *p << endl;
    // a1.say();
    // a1 = cat;
    // a1.say();
    // Animal a(cat);
    // Cat xx;
    // xx = cat;
    // xx.say();

    return 0;
}