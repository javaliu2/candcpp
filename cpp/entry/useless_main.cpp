#include <iostream>
#include "useless.h"
using namespace std;

int main() {
    Useless one(5, 'x');
    Useless two = one;  // 调用拷贝构造
    Useless three(5, 's');
    Useless four(one + three);  // 调用移动构造

    cout << "object one: ";
    one.ShowData();
    cout << "object two: ";
    two.ShowData();
    cout << "object three: ";
    three.ShowData();
    cout << "object four: ";
    four.ShowData();
    return 0;
}
