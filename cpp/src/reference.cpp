#include <iostream>
#include <type_traits>
#include <string>
using namespace std;

template<typename T>
void func(T&& param) {
    // param类型可以为左值引用类型，或者右值引用类型
    // 但是其本身是左值
    if (std::is_same<decltype(param), string>::value)
        std::cout << "string" << std::endl;
    else if (std::is_same<decltype(param), string&>::value)
        std::cout << "string&" << std::endl;
    else if (std::is_same<decltype(param), string&&>::value)
        std::cout << "string&&" << std::endl;
    else if (std::is_same<decltype(param), int>::value)
        std::cout << "int" << std::endl;
    else if (std::is_same<decltype(param), int&>::value)
        std::cout << "int&" << std::endl;
    else if (std::is_same<decltype(param), int&&>::value)
        std::cout << "int&&" << std::endl;
    else
        std::cout << "unkown" << std::endl;
}

int getInt() {
    return 10;
}

void other_func(int& x) {
    cout << "左值版本" << endl;
}
void other_func(int&& x) {
    cout << "右值版本" << endl;
}

template <typename T>
void forward_func(T&& arg) {
    // arg本身值左值
    // other_func(arg);
    other_func(std::forward<T>(arg));
}

/**
 * output: 
 * int
 * int&
 * int
 */
int main() {
    // 1. 实参是 左值（int变量）
    int a = 10;
    func(a);  // 输出：int&

    // 2. 实参是 右值（int字面量）
    func(20);  // 输出：int&&

    // 3. 实参是 左值引用（int&）
    int& ref_a = a;
    func(ref_a);  // 输出：int&（左值引用的实参，仍按左值推导）

    // 4. 实参是 右值引用（int&&，但本身是左值）
    int&& rref_a = move(a);
    func(rref_a);  // 输出：int&（rref_a是有名字的左值，按左值推导）

    // 5. 实参是 右值（std::move转换后的左值）
    func(move(a));  // 输出：int&&

    // 6. string类型同理
    string s = "test";
    func(s);                // 输出：string&（左值string）
    func("temp");           // 输出：string（字符串字面量→临时string，右值）
    func(move(s));          // 输出：string&&（move后的string，右值）

    cout << "====" << endl;
    // 变量，有两个属性，分别是变量的类型和变量的值
    int x = 1;
    func(1); // 传递参数是右值 T推导成了int, 所以是int&& param, 右值引用
    func(x); // 传递参数是左值 T推导成了int&, 所以是int&&& param, 折叠成 int&,左值引用
    func(getInt());// 参数getInt()返回值是右值 T推导成了int, 所以是int&& param, 右值引用
    int&& x_right_ref = std::move(x);
    // x_right_ref是一个变量，他的类型是右值引用，他的值是一个右值
    func(x_right_ref);  // int&, 右值引用类型变量的值是左值
    func(std::move(x_right_ref));  // int
    int&& const_right_ref = 2;
    func(const_right_ref);  // int&

    std::cout << "=====" << std::endl;
    int aa = 10;
    int& aa_ref = aa;
    cout << "address of aa: " << &aa << std::endl;  // 0x7ffe4a5382e4
    cout << "address of aa_ref: " << &aa_ref << endl;  // 0x7ffe4a5382e4
    int&& aa_r_ref = move(aa);  // 只是一个类型转换工具，不会改变aa的内存地址，也不会清空aa的值，
    // 只是让aa具备了右值属性，允许被移动语义使用
    cout << "address of aa_r_ref: " << &aa_r_ref << std::endl;  // 0x7ffe4a5382e4
    aa_r_ref = 20;  // 我不能理解，为什么可以修改
    cout << "aa: " << aa << endl; // 20
    int&& t_r_ref = 5;
    cout << "t_r_ref: " << t_r_ref << endl;
    cout << "address of t_r_ref: " << &t_r_ref << endl;
    cout << "====" << endl;
    int c = 10;
    forward_func(5);  // 左值版本
    forward_func(move(c));  // 左值版本
    forward_func(c);
    return 0;
}

