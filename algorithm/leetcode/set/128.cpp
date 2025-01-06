#include <vector>
#include <unordered_set>
using namespace std;

class Solution {
public:
    /**
     * 思路: LeetCode Official
     * 链接: https://leetcode.cn/problems/longest-consecutive-sequence/
     * solutions/276931/zui-chang-lian-xu-xu-lie-by-leetcode-solution/?
     * envType=study-plan-v2&envId=top-100-liked
     * @author xs
    */
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> set;
        for (const int& ele : nums) {
            set.insert(ele);
        }
        int ans = 0;
        for (int ele : set) {
            // 以ele为起点的序列 肯定 不会比以ele-1为起点的序列更长
            if (set.count(ele - 1)) {
                continue;
            }
            int t = 1;
            while (set.count(ele + t)) {
                ++t;
            }
            ans = max(ans, t);
        }
        return ans;
    }
};