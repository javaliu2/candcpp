#include "student.h"
#include <iostream>
using namespace std;

int main(){
    Student xs("xs", 24, "just enjoy now");
    Student mi("maomi", 23, "brother");
    cout << "in main, address of mi: " << &mi << endl;
    xs.non_const_func(mi);
    // // 对象xs被const修饰，那么它只能调用被const修饰的方法
    // //xs.~Student();  // 析构函数也是可以主动调用的
    // xs.sayHello();
    // Student maomi = xs; // 拷贝构造
    // //另一种写法
    // // Student maomimi(xs);
    // Student dahei("dahei", 25, "heiheihei");
    // dahei.sayHello();
    // dahei = xs;
    // dahei.sayHello();

    //const Student o;
    return 0;
}