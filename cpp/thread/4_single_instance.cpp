#include <iostream>

using namespace std;

class Log {
private:
    Log() {}
    Log(const Log& log) = delete;
    Log& operator=(const Log& log) = delete;
public:
    static Log& getInstance(){
        static Log* log_ptr = nullptr;
        if (!log_ptr) {
            log_ptr = new Log();
        }
        return *log_ptr;
    }
    void log_print(string msg) {
        cout << __TIME__ << " " << msg << endl;
    }
};

int main() {
    // Log log1 = Log::getInstance();
    // cout << "log1 address: " << &log1 << endl;
    // Log log2 = Log::getInstance();
    // cout << "log2 address: " << &log2 << endl;
    // output:
    // log1 address: 0x7ffe97831986
    // log2 address: 0x7ffe97831987
    // 因为没有禁用拷贝构造，发生了拷贝构造，所以就是多个Log对象了

    Log& log3 = Log::getInstance();
    cout << "log3 address: " << &log3 << endl;
    Log& log4 = Log::getInstance();
    cout << "log4 address: " << &log4 << endl;
    return 0;
}