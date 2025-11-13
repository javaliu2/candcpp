#include <iostream>
#include <vector>
using namespace std;

class Solution {
private:
    int gcd(int a, int b) {
        if (b == 0) {
            return a;
        }
        return gcd(b, a % b);
    }
public:
    /**
    * idea from lingshen
    */
    int minOperations(vector<int>& nums) {
        int g = 0, cnt1 = 0, n = nums.size();
        // gcd(0, a) == a
        for (int num : nums) {
            g = gcd(g, num);
            if (num == 1) {
                cnt1++;
            }
        }
        // 所有元素的gcd不为1，那么不可能将所有元素全部变为1
        if (g > 1) {
            return -1;
        }
        // nums中已经有1了，那么将所有元素变为1的操作次数为nums.size()-1的个数
        if (cnt1) {
            return n - cnt1;
        }
        // 针对测试用例[2, 6, 3, 4]
        // 以2打头的最短连续子数组为[2, 6, 3]，更新min_size
        // 以6打头的最短连续子数组为[6, 3, 4]，min_size保持不变
        // 以3打头的最短连续子数组为[3, 4]，更新min_size
        // 寻找全局最短连续子数组，满足所有元素的gcd为1
        int min_size = n;
        for (int i = 0; i < n; ++i) {
            g = 0;
            for (int j = i; j < n; ++j) {
                g = gcd(g, nums[j]);
                if (g == 1) {
                    min_size = min(min_size, j - i);
                    break;  // 满足要求的最短连续子数组，因为后边j++了，不可能比第一次满足要求的j-i更小
                }
            }
        }
        // 长度为j-i+1的连续子数组需要(j-i+1-1=j-i)次操作变出一个1，
        // 总的操作次数为j-i+n-1
        return min_size + n - 1;
    }
};