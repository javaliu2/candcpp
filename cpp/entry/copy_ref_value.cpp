#include <iostream>
#include <string>
#include <cstring>
using namespace std;


class Person {
    public:
        int age;
        char * name;
        string motto;
    public:
        Person() : age(0), motto(""), name(nullptr) {
        }
        Person(const char * name, int age, string motto) : age(age), motto(motto) {
            size_t len = strlen(name) + 1;
            name = new char[len];
            strcpy(this->name, name);
        }
        Person(const Person& p) {
            // 注意，这是构造函数，当前对象还没被构造出来，对未初始化指针变量执行delete操作引发crash
            //delete[] name;
            size_t len = strlen(p.name) + 1;
            name = new char[len];
            strcpy(name, p.name);
            age = p.age;
            motto = p.motto;
        }
        Person& operator=(const Person& p) {
            // 能给当前对象赋值，那么它一定是构造完毕的
            //（如果是无参构造，那么name也有nullptr值，调用delete不会crash）
            if (this == &p) {
                return *this;
            }
            // 赋值运算符重载需要释放旧有资源以防止出现内存泄漏
            delete[] name;
            size_t len = strlen(p.name) + 1;
            name = new char[len];
            strcpy(name, p.name);
            age = p.age;
            motto = p.motto;
            return *this;
        }

};

int main() {
    // 1、默认拷贝构造按照成员变量各自的特性进行拷贝
    // 基本类型，按照value，如果包含其他对象，那么按照该对象自己实现的拷贝构造完成对该成员变量的拷贝
    Person p("xiaosong", 24, "enjoy it");
    cout << "addrss of elements in p: " << endl;
    cout << "address of age: " << &p.age << endl; 
    cout << "address of motto: " << &p.motto << endl; 
    cout << "address of name: " << &p.name << endl;
    cout << p.motto << endl; 
    printf("name: %d\n", p.age); 
    Person pp(p);
    cout << "after Person pp(p)" << endl;
    cout << "addrss of elements in pp: " << endl;
    cout << "address of age: " << &pp.age << endl; 
    cout << "address of motto: " << &pp.motto << endl; 
    cout << "address of name: " << &pp.name << endl; // 和上面 &p.name 肯定是不一样的，因为这是对象内存
    // 但是他们的值是一样的，即指向同一块内存
    // cout << p.name << " ?= " << pp.name << endl;
    printf("p.name %p\n", p.name);
    printf("pp.name %p\n", pp.name);
    // printf("name: %s", p.name);

    Person xr;
    xr = p;
    return 0;
}