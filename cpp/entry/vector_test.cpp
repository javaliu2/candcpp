#include "vector.h"
#include "student.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv){
    // 1、test append、cout、
    Vector<int> v1, v2;
    v1.append(5);
    v1.append(2);
    v1.append(1);
    cout << v1;
    cout << "size: " << v1.getSize() << "; capacity :"<< v1.getCapacity() << endl;
    v1.append(4);
    cout << v1;
    cout << "size: " << v1.getSize() << "; capacity :"<< v1.getCapacity() << endl;
    // 2、test copy constructor
    Vector<int> cpy(v1);
    cout << cpy;
    // 3、test sort
    v1.append(-1);
    v1.append(-6);
    v1.append(17);
    v1.append(4);
    cout << v1;
    v1.sort();
    cout << v1;
    // 4、user-defined data: Student
    Vector<Student> stus(10);
    stus.append(Student("xs", 24, "now"));
    stus.append(Student("cr", 23, "xiaorui"));
    stus.append(Student("hx", 24, "xiyuansi"));
    stus.append(Student("laofan", 24, "shutu"));
    stus.append(Student("chenguang", 24, "goodbro"));
    cout << stus;
    cout << "===============\n";
    // 5、sort list of Student
    stus.sort();
    cout << stus;
    return 0;
}