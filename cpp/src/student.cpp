#include "student.h"
#include <string>
#include <iostream>

using namespace std;

Student::Student(){
    cout << "无参构造" << endl;
}

Student::Student(string name, int age, string motto) : name(name), age(age), motto(motto) {
    cout << "有参构造" << endl;
}

Student::Student(const Student& stu){
    cout << "拷贝构造" << endl;
    name = stu.name;
    age = stu.age;
    motto = stu.motto;
}

Student::~Student(){
    cout << "析构函数" << endl;
}

void Student::sayHello() {
    cout << "non const" << endl;
}

Student& Student::operator=(const Student& stu) {
    cout << "赋值运算" << endl;
    if (this == &stu) {
        return *this;
    }
    name = stu.name;
    age = stu.age;
    motto = stu.motto;
    return *this;
}
void Student::non_const_func(Student& stu)
{
    cout << "in func, address of parameter stu: " << &stu << endl;
}

bool Student::operator<(const Student& other) { 
    // 按照name排序
    return name < other.name; 
}

void Student::sayHello() const{
    /*
    const修饰的方法调用非const方法报错：
    the object has type qualifiers that are not compatible with the member function "Student::non_const_func" 
    也就是说，方法签名后面带const，即限制this为const this，不能通过this修改对象属性
    */
    //non_const_func();
    cout << "i am " << name << ", and i am " << age << " years old, my motto is " << motto << endl;
}

ostream& operator<<(ostream& os, const Student& stu) {
    cout << "i am " << stu.name << ", and i am " << stu.age << " years old and my motto is " << stu.motto << endl;
    return os;
}