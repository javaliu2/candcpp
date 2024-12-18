#include <iostream>
#include "fraction.h"
#include "student.h"

using namespace std;

int main() {
    Fraction a, b;
    string str = "23";
    // 使用构造函数Fraction(const std::string& str)构造临时对象，然后使用operator=完成赋值
    // a = str;  
    // 完成类型转换，string==>Fraction

    // 如果不想编译器完成自动类型转换，可以声明以上构造函数为explicit，
    // 这样转换的时候，需要显式调用构造函数:
    a = Fraction(str);
    cout << a; 
    double d = a;  // 调用operator double函数
    cout << "double: " << d << endl;
    string str2 = a;  // 调用operator string函数
    cout << "string: " << str2 << endl;
    Student stu = a; // 调用operator Student函数
    // 但是，并不推荐使用operator，而是写成函数比如：
    // toDouble()、toString、toStudent，显式调用，这样易读性高
    return 0;
}