#include <vector>
#include <climits>

using namespace std;

class Solution {
public:
    int minimumSubarrayLength_bf(vector<int>& nums, int k) {
        // brute force
        int ans = 100, t, j, n = nums.size();
        for (int i = 0; i < n; i++) {
            t = nums[i];
            j = i + 1;
            while (j < n && t < k) {
                t |= nums[j];
                j++;
            }
            if (t >= k) {
                ans = min(ans, j - i);
            }
        }
        return ans == 100 ? -1 : ans;
    }
    int func(int bits[], int v, bool choice) {
        int cnt = 0;
        while (v) {
            if (v & 1) {
                if (choice) {
                    bits[cnt]++;
                } else {
                    bits[cnt]--;
                }
            }
            v >>= 1;
            cnt++;
        }
        // 计算bits所代表的数的大小
        int value = 0;
        for (int i = 0; i < 30; i++) {
            if (bits[i]) {
                value |= 1 << i;
            }
        }
        return value;
    }
    /**
     * sliding window
     */
    int minimumSubarrayLength_sw(vector<int>& nums, int k) {
        int bits[30] = {0};
        int value = 0, l = 0, r = 0, n = nums.size(), ans = INT_MAX;
        while (r < n) {
            value = func(bits, nums[r], true);
            while (l <= r && value >= k) {
                ans = min(ans, r - l + 1);
                value = func(bits, nums[l], false);
                l++;
            }
            r++;            
        }
        return ans == INT_MAX ? -1 : ans;
    }
};