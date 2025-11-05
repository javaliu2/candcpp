#include <iostream>
#include <functional>
#include <memory>

void printHello() {
    std::cout << "Hello, std::bind + std::function!" << std::endl;
}

void printSum(int a, int b) {
    std::cout << "sum of a and b: " << a+b << std::endl;
}

int sum(int a, int b) {
    return a + b;
}

class Calculator {
public:
    Calculator() : count(0) {}
    // 非静态成员函数（隐含 this 指针参数）
    void multiply(int x, int y) {
        count += x * y;
        std::cout << "Multiply Result: " << x * y << std::endl;
    }

    // 静态成员函数（无 this 指针，类似普通函数）
    static void divide(int x, int y) {
        std::cout << "Divide Result: " << x / y << std::endl;
    }
    int getCount() const {
        return count;
    }
    Calculator* operator&() {
        return this + 1;  // 不是原对象的真实地址
    }
private:
    int count;
};

int main () {
    std::function<void()> task = std::bind(printHello);
    task();
    int a = 3, b = 4;
    std::function<void()> task2 = std::bind(printSum, a, b);
    task2();
    // 使用占位符，延迟传参
    auto bind_printSum = std::bind(printSum, std::placeholders::_1, std::placeholders::_2);
    std::function<void()> task3 = std::bind(bind_printSum, 4, 5);
    task3();
    std::function<void(int, int)> task4 = std::bind(printSum, std::placeholders::_1, std::placeholders::_2);
    task4(7, 8);

    Calculator calc;  // 类实例（非静态成员函数需要）

    // 一、绑定非静态成员函数（必须传入对象/指针，作为 this 指针）
    // 1. 用对象引用绑定（std::ref 避免拷贝）
    std::function<void()> cal11 = std::bind(&Calculator::multiply, calc, 8, 4);  // 发生拷贝构造，cal11中的是拷贝的calc对象
    cal11();
    std::cout << "count: " << calc.getCount() << std::endl;
    std::function<void()> cal1 = std::bind(&Calculator::multiply, std::ref(calc), 8, 4);  // 引用传递，cal1中的是main中的calc对象
    cal1();
    std::cout << "count: " << calc.getCount() << std::endl;

    // 2. 用对象指针绑定
    Calculator* calc_ptr = &calc;
    std::function<void()> cal2 = std::bind(&Calculator::multiply, calc_ptr, 10, 5);
    cal2();

    // 二、绑定静态成员函数（无需传入对象，直接绑定，类似普通函数）
    std::function<void()> cal3 = std::bind(&Calculator::divide, 15, 3);
    cal3();

    std::cout << "普通&obj: " << &calc << std::endl;
    std::cout << "std::address(obj): " << std::addressof(calc) << std::endl;  // 永远获取到calc的真实地址
    
    std::function<int()> sum_ = std::bind(sum, 3, 4);
    sum_();
    std::function<int(int, int)> sum_2 = sum;  // 无需绑定
    sum_2(4, 5);
    return 0;
}
