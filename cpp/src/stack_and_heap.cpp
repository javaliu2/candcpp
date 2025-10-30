#include <iostream>
#include <string>

class Person{
    private:
        int age;
        std::string name;
    public:
        Person(){}
        Person(int age, std::string name):name(name),age(age){}
};

Person* func() {
    Person* p = new Person(25, "xs");
    std::cout << "in func(), address of p is " << &p << std::endl;
    std::cout << "in func(), value of p is " << p << std::endl;
    return p;
}
/**
 * output:
 * in func(), address of p is 0x7fff03a44a98 // func中局部指针变量p的地址（栈上）
 * in func(), value of p is 0x6325eb68bd20  // p所指向对象的地址（堆上）
 * in main(), address of cr is 0x7fff03a44b20  // main中cr对象的地址（栈上）
 * in main(), address of p is 0x7fff03a44af8  // main中局部指针变量p的地址（栈上）
 * in main(), value of p is 0x6325eb68bd20  // main中指针变量p所指向对象的地址（堆上）
 * 栈是高地址，堆是低地址
 */
int main() {
    // p在栈上，p所指向的对象在堆上
    // cr对象在栈上
    // p的值是其所指向对象的地址
    Person* p = func();
    Person cr(25, "cr");
    std::cout << "in main(), address of cr is " << &cr << std::endl;
    std::cout << "in main(), address of p is " << &p << std::endl;
    std::cout << "in main(), value of p is " << p << std::endl;
    delete p;  // 堆内存需要手动释放，否则会造成内存泄漏
    return 0;
}

