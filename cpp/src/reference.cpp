#include <iostream>
#include <type_traits>
#include <string>
using namespace std;

template<typename T>
void func(T&& param) {
    // param类型可以为左值引用类型，或者右值引用类型
    // 但是其本身是左值
    if (std::is_same<string, T>::value)
        std::cout << "string" << std::endl;
    else if (std::is_same<string&, T>::value)
        std::cout << "string&" << std::endl;
    else if (std::is_same<string&&, T>::value)
        std::cout << "string&&" << std::endl;
    else if (std::is_same<int, T>::value)
        std::cout << "int" << std::endl;
    else if (std::is_same<int&, T>::value)
        std::cout << "int&" << std::endl;
    else if (std::is_same<int&&, T>::value)
        std::cout << "int&&" << std::endl;
    else
        std::cout << "unkown" << std::endl;
}

int getInt() {
    return 10;
}
/**
 * output: 
 * int
 * int&
 * int
 */
int main() {
    // 变量，有两个属性，分别是变量的类型和变量的值
    int x = 1;
    func(1); // 传递参数是右值 T推导成了int, 所以是int&& param, 右值引用
    func(x); // 传递参数是左值 T推导成了int&, 所以是int&&& param, 折叠成 int&,左值引用
    func(getInt());// 参数getInt()返回值是右值 T推导成了int, 所以是int&& param, 右值引用
    int&& x_right_ref = std::move(x);
    // x_right_ref是一个变量，他的类型是右值引用，他的值是一个右值
    func(x_right_ref);  // int&, 右值引用的本质是左值
    func(std::move(x_right_ref));  // int
    int&& const_right_ref = 2;
    func(const_right_ref);  // int&

    typedef int& LRef;
    typedef int&& RRef;

    LRef&  a = x;  // 等价于 int& & → 折叠为 int&
    LRef&& b = x;  // 等价于 int& && → 折叠为 int&
    RRef&  c = x;  // 等价于 int&& & → 折叠为 int&
    RRef&& d = 3;  // 等价于 int&& && → 折叠为 int&&
    std::cout << "=====" << std::endl;
    func(a);
    func(b);
    func(c);
    func(d);
    return 0;
}