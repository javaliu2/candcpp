#include <bits/stdc++.h>

using namespace std;

/**
 * @author 灵神
 * 思路: https://leetcode.cn/problems/remove-duplicates-from-sorted-array-ii/solutions/3060042/yong-zhan-si-kao-yuan-di-shi-xian-python-zw8l/
 */
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int stackSize = 2, n = nums.size();
        for (int i = 2; i < n; i++) {
            if (nums[i] == nums[stackSize - 2]) {
                continue;
            }
            nums[stackSize] = nums[i];
            stackSize++;
        }
        return min(stackSize, n);
    }
};