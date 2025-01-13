#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
    /* 219ms */
    vector<int> twoSum_bf(vector<int>& nums, int target) {
        vector<int> ans;
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = 0; j < nums.size(); ++j) {
                if (j == i) {
                    continue;
                }
                if (nums[j] + nums[i] == target) {
                    ans.push_back(i);
                    ans.push_back(j);
                    return ans;
                }
            }
        }
        return ans;
    }
    /**
     * @author LeetCode Official
     */
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> ans;
        unordered_map<int, int> hashtable;
        for (int i = 0; i < nums.size(); ++i) {
            auto it = hashtable.find(target - nums[i]);
            if (it != hashtable.end()) {
                ans.push_back(it->second);
                ans.push_back(i);
                break;
            }
            hashtable[nums[i]] = i;
        }
        return ans;
    }

    vector<int> twoSum_tp(vector<int>& nums, int target) {
        vector<int> ans, original(nums);
        int l = 0, r = nums.size() - 1;
        sort(nums.begin(), nums.end());
        // 需要用一个map记录排序前元素的索引和排序后对应索引的映射
        // 可以实现定制版的排序，记录索引的映射
        while (l < r) {
            int sum = nums[l] + nums[r];
            if (sum == target) {
                // 这里使用笨方法，再去搜索原始数组中元素所在位置
                // 这家伙，缝缝补补补
                // 有点理解适配器了，转化一个问题使其成为已解决问题的模板
                bool flag = false, flag2 = false;
                for (int i = 0; i < nums.size(); ++i) {
                    if (!flag && original[i] == nums[l]) {
                        ans.push_back(i);
                        flag = true;
                        continue;
                    }
                    if (!flag2 && original[i] == nums[r]) {
                        ans.push_back(i);
                        flag2 = true;
                    }
                    if (flag && flag2) {
                        break;
                    }
                }
                break;
            } else if (sum < target) {
                l++;
            } else {
                r--;
            }
        }
        return ans;
    }
};