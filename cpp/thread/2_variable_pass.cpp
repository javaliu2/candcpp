#include <iostream>
#include <string>
#include <thread>
using namespace std;

class A {
    public:
        void func() {
            cout << "Thread " << this_thread::get_id << " start" << endl;
            cout << "Thread " << this_thread::get_id << " finish" << endl;
        }
};

void func(int& x) {
    x++;
}
void func_ptr(int* x_ptr) {
    cout << "in func_ptr(), " << *x_ptr << endl;
}

int main() {
    // 1、临时变量
    // thread t(func, 1);  // 临时变量不能传递给引用类型的参数变量
    int x = 1;
    thread t(func, ref(x));
    if (t.joinable()) {
        t.join();
    }
    cout << x << endl;
    // 2、传递局部变量的指针或者引用
    int local_x = 2;
    thread t2(func_ptr, &local_x);
    if (t2.joinable()) {
        t2.join();  // 因为有join，所以可以保证线程t2中local_x不会失效
    }
    cout << "local_x: " << local_x << endl;
    // 3、类成员函数作为入口函数，类对象被提前释放
    A* a = new A();
    thread t3(&A::func, a);  // 隐含参数: which对象
    delete a;  // 主动删除对象a，会造成crash
    t3.join();
    return 0;
}
