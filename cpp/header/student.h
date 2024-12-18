#ifndef STUDENT_H_
#define STUDENT_H_
#include <string>
#include <iostream>
using namespace std;

class Student {
    private:
    /*
    const成员变量必须在构造函数初始化列表中初始化
    "Student::Student()" provides no initializer for:C/C++(366)
    student.cpp(7, 19): const member "Student::i" */
        // const int i;
        string name;
        int age;
        string motto;
    public:
        Student();
        Student(string name, int age, string motto);
        Student(const Student& sut);
        ~Student();
        /* 以下两个方法构成重载，因为
        without const ==> sayHello(Student* this)
        with const    ==> sayHello(const Student* this)
        parameter不一样，故构成重载
        */ 
        void sayHello();
        void sayHello() const;
        // 通过显式指定赋值运算符方法为delete，那么编译器就不会提供默认的实现了
        // Student& operator=(const Student& stu) = delete;
        Student& operator=(const Student& stu);
        void non_const_func(Student& stu);

        friend ostream& operator<<(ostream& os, const Student& stu);
        bool operator<(const Student& other);
};

#endif