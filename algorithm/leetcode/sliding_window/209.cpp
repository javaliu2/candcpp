#include <algorithm>
#include <vector>
using namespace std;

class Solution {
   public:
    /**
     * @author 灵神
     */
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size(), ans = n + 1, sum = 0, left = 0;
        for (int right = 0; right < n; ++right) {  // 枚举子数组右端点
            sum += nums[right];
            while (sum - nums[left] >= target) {  // 尽量缩小子数组长度
                sum -= nums[left++];              // 左端点右移
            }
            if (sum >= target) {
                ans = min(ans, right - left + 1);
            }
        }
        return ans <= n ? ans : 0;
    }
};