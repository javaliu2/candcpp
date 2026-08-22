#include <string>
#include <iostream>
using namespace std;

/**
 * 引用变量一旦初始化，就不能改变其引用的对象（不能重新绑定），但是可以改变引用对象的内容。
 */
void func(string& str) {
    string str2("123");
    str = str2;  // operator=()，内容的赋值，内容的改变
    // str[0] = '8';  // 这也是内容的改变
}
void func2() {
    // const修饰谁，就看它左边，如果左边没有东西，就看它右边
    // 这两个是等价的
    const char *p = NULL;
    p = new char[3];
    // p[0] = '0';
    char const *pp = NULL;
    pp = new char[3];
    // pp[0] = '0';

    char *const p2 = NULL;
    // p2 = new char[3];
    p2[0] = '0';
    

    const char *const p3 = NULL;
    // p3 = new char[3];
    // p3[0] = '0';
}
int main() {
    string a("1234");
    cout << a << endl;
    func(a);
    cout << a << endl;

    return 0;
}