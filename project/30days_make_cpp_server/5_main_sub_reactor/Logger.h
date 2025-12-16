#pragma once
#include <iostream>
#include <mutex>
#include <chrono>
#include <sstream>
#include <thread>
#include <iomanip>

class Logger {
public:
    template<typename... Args>  // 可变参数，其中每一个参数都是万能引用
    static void log(Args&&... args) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << timestamp() << "[tid " << std::this_thread::get_id() << "] ";
        (std::cout << ... << args) << "\n";
    }
private:
    static std::string timestamp() {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto t = system_clock::to_time_t(now);
        std::tm tm{};
        localtime_r(&t, &tm);
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "." << std::setw(3) << std::setfill('0') << ms.count();
        return oss.str();
    }
private:
    static std::mutex mutex_;  // 只是声明
};