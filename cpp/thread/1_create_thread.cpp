#include <iostream>
#include <string>
#include <thread>
using namespace std;

void print(const string& name) {
    cout << "i am " << name << endl;
}

void age_increment(int& age) {
    ++age;
}

int main() {
    thread t(print, "xs");
    if (t.joinable()) {
        t.join();  //  主线程阻塞等待线程t的完成
    }

    int age = 25;
    thread t2(age_increment, ref(age));  // 需要使用ref来包装变量
    if (t2.joinable()) {
        t2.join();
        cout << "age: " << age << endl;
    }
    return 0;
}