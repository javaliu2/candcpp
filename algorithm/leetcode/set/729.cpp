#include <iostream>
#include <set>
#include <vector>
using namespace std;

class MyCalendar {
   private:
    set<pair<int, int>> st;  // schedule_table
   public:
    MyCalendar() {}

    /**
     * failure version. 不想写了，想不明白，费劲
     */
    bool book(int startTime, int endTime) {
        if (st.size() == 0) {
            st.insert({startTime, endTime});
            return true;
        }
        if (st.size() == 1) {
            if (st.begin()->second > startTime) {
                return false;
            }
            st.insert({startTime, endTime});
            return true;
        }
        // 找到大于start的第一个pair
        auto pre = st.end();
        for (auto iter = st.begin(); iter != st.end();) {
            if (iter->first < startTime) {
                pre = iter++;
            } else {
                if (pre != st.end()) {
                    if (startTime < pre->second || endTime > iter->first) {
                        return false;
                    }
                } else if (endTime > iter->first) {
                    return false;
                }
                break;
            }
        }
        st.insert({startTime, endTime});
        return true;
    }
};
/**
 * LeetCode Official Solution(1)
 * 还是逻辑没想清楚，假设存在两个区间[s1, e1), [s2, e2),两者不相交的满足条件是
 * s2 >= e1 或者 e2 <= s1，那么相交的情况就是 s2<e1且e2>s1
 */
class MyCalendar2 {
    vector<pair<int, int>> booked;

   public:
    bool book(int start, int end) {
        for (auto &[l, r] : booked) {
            if (l < end && start < r) {
                return false;
            }
        }
        booked.emplace_back(start, end);
        return true;
    }
};

int main() {
    // set<pair<int, int>> st;
    // st.insert({2, 5});
    // st.insert({1, 4});
    // st.insert({4, 5});
    // st.insert({-1, 5});
    // set<pair<int, int>>::iterator iter = st.begin();
    // while (iter!=st.end()) {
    //     // 指针直接访问
    //     cout << "(" << iter->first << "," << iter->second << ")\n";
    //     // 解引用，然后通过'.'运算符访问
    //     cout << "(" << (*iter).first << "," << (*iter).second << ")\n";
    //     ++iter;
    // }
    MyCalendar my;
    bool res;
    res = my.book(47, 50);
    cout << res << endl;
    res = my.book(33, 41);
    cout << res << endl;
    res = my.book(39, 45);
    cout << res << endl;
    res = my.book(33, 42);
    cout << res << endl;
    return 0;
}