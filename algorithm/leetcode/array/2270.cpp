#include <vector>
#include <numeric>

using namespace std;

class Solution {
public:
    int waysToSplitArray(vector<int>& nums) {
        // 前缀和
        int n = nums.size();
        vector<long long> sums(n);
        sums[0] = nums[0];
        for (int i = 1; i < n; ++i) {
            sums[i] = sums[i - 1] + nums[i];
        }
        // 枚举下标, [0, ..., n-2]判断是否合法
        long long lastsum = sums[n - 1];
        int ans = 0;
        for (int i = 0; i < n - 1; ++i) {
            if (sums[i] >= lastsum - sums[i]) {
                ans++;
            }
        }
        return ans;
    }
};
class Solution2 {
public:
    /**
     * @author LeetCode Official
     */
    int waysToSplitArray(vector<int>& nums) {
        int n = nums.size();
        long long left = 0, right = accumulate(nums.begin(), nums.end(), 0LL);
        int ans = 0;
        for (int i = 0; i < n - 1; ++i) {
            left += nums[i];
            right -= nums[i];
            if (left >= right) {
                ++ans;
            }
        }
        return ans;
    }
};
