#include <vector>
#include <ranges>
using namespace std;

class Solution {
public:
    int semiOrderedPermutation(vector<int>& nums) {
        // 直观想法
        int n = nums.size();
        if (nums[0] == 1 && nums[n - 1] == n) {
            return 0;
        }
        if (nums[0] == 1) {
            // n距离最后一个元素之间的偏移
            for (int i = 0; i < n; i++) {
                if (nums[i] == n) {
                    return n - 1 - i;
                }
            }
        }
        if (nums[n-1] == n) {
            // 1距离第一个元素的偏移
            for (int i = 0; i < n; i++) {
                if (nums[i] == 1) {
                    return i;
                }
            }
        }
        // 1和n的位置
        // 1_idx > n_idx，1的偏移+n的偏移-1
        // 否则，1的偏移+n的偏移
        int one_idx, n_idx;
        for (int i = 0; i < n; i++) {
            if (nums[i] == 1) {
                one_idx = i;
            }
            if (nums[i] == n) {
                n_idx = i;
            }
        }
        if (one_idx > n_idx) {
            return one_idx + n - 1 - n_idx - 1;
        }
        return one_idx + n - 1 - n_idx;
    }
    /**
     * @author 灵神
     */
    // int semiOrderedPermutation_ling(vector<int>& nums) {
    //     auto [p, q] = ranges::minmax_element(nums);
    //     return p - q + nums.size() - 1 - (p > q);
    // }
};

    