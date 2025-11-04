#include <iostream>
#include <vector>
#include <string>

using namespace std;
class Person {
private:
    string name;
    int age;
public:
    Person(string name, int age) : name(name), age(age) {
        cout << "default constructor" << endl;
    }
    Person(const Person& other) : name(other.name), age(other.age) {
        cout << "copy constructor" << endl;
    }
    Person(Person&& other) noexcept : name(other.name), age(age) {
        cout << "move constructor" << endl;
    }
};
/**
 * output:
 * push_back调用
 * default constructor
 * move constructor
 * emplace_back调用
 * default constructor
 */
int main() {
    vector<Person> vec;
    vec.reserve(10);
    cout << "push_back调用" << endl;
    vec.push_back(Person("xs", 25));
    cout << "emplace_back调用" << endl;
    vec.emplace_back("cr", 25);  // 直接在vec内存中构造，无临时对象
    return 0;
}