#include <vector>

using namespace std;

class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        /**
        * 前缀和可以实现吗？
        * sum[j] := sum[0,...,j]
        * 枚举所有子数组，判断其和是否为k
        */
        int n = nums.size(), ans = 0;  // 千记万记，变量要初始化
        vector<int> sum(n);
        sum[0] = nums[0];
        for (int i = 1; i < n; ++i) {
            sum[i] = sum[i - 1] + nums[i];
        }
        for (int i = 0; i < n; ++i) {
            if (nums[i] == k) {
                ans++;
            }
            for (int j = i + 1; j < n; ++j) {
                int sub_num = i - 1 < 0 ? 0 : sum[i - 1];
                if (sum[j] - sub_num == k) {
                    ans++;
                }
            }
        }
        return ans;
    }
};