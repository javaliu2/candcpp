#include <algorithm>
#include <unordered_map>

using namespace std;

class Solution {
   public:
    int countBalls(int lowLimit, int highLimit) {
        // 使用hashmap记录每个盒子中球的数量，最后返回所有value中的最大值
        unordered_map<int, int> cnts;
        for (int i = lowLimit; i <= highLimit; i++) {
            int bit_sum = 0, t = i;
            while (t) {
                bit_sum += t % 10;
                t /= 10;
            }
            cnts[bit_sum]++;  // 如果bit_sum不存在，operator++()会插入bit_sum键赋值为0，然后加1
        }
        // 使用 max_element 寻找 value 中的最大值
        auto max_it = std::max_element(
            cnts.begin(), cnts.end(),
            [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                return a.second < b.second;  // 比较 value
            });
        return max_it->second;
    }
};