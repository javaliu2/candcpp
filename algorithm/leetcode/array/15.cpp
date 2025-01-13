#include <algorithm>
#include <vector>
using namespace std;

class Solution {
   public:
    /**
     * 超时, 308/313
     */
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> ans;
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                for (int k = j + 1; k < n; ++k) {
                    if (nums[i] + nums[j] + nums[k] == 0) {
                        vector<int> t = {nums[i], nums[j], nums[k]};
                        sort(t.begin(), t.end());
                        vector<vector<int>>::iterator v = ans.begin();
                        while (v != ans.end()) {
                            if ((*v)[0] == t[0] && (*v)[1] == t[1] &&
                                (*v)[2] == t[2]) {
                                break;
                            }
                            ++v;
                        }
                        if (v == ans.end()) {
                            ans.push_back(t);
                        }
                    }
                }
            }
        }
        return ans;
    }
    vector<vector<int>> twoSum(vector<int>& nums, int l, int r, int target,
                               int first_ele) {
        vector<vector<int>> ans;
        while (l < r) {
            int sum = nums[l] + nums[r];
            if (sum == target) {
                vector<int> t = {first_ele, nums[l], nums[r]};
                ans.push_back(t);
                while (l < r && nums[l] == nums[l + 1]) {
                    l++;
                }
                l++;
                while (l < r && nums[r] == nums[r - 1]) {
                    r--;
                }
                r--;
            } else if (sum < target) {
                l++;
            } else {
                r--;
            }
        }
        return ans;
    }
    /**
     * @author LeetCode Official
     */
    vector<vector<int>> threeSum(vector<int>& nums) {
        // 对数组排序，枚举第一个数，相同值跳过，然后问题就变为找和为target的两数之和的子问题
        sort(nums.begin(), nums.end());
        int n = nums.size();
        vector<vector<int>> ans;
        for (int i = 0; i < n; ++i) {
            if (i > 0 && nums[i] == nums[i - 1]) {
                continue;
            }
            auto t = twoSum(nums, i + 1, n - 1, -nums[i], nums[i]);
            ans.insert(ans.end(), t.begin(), t.end());
        }
        return ans;
    }
};