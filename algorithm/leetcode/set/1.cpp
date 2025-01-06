#include <vector>
#include <unordered_map>

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
};