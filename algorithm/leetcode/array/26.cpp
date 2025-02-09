#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        stack<int> s;
        s.push(nums[0]);
        for (int i = 1; i < nums.size(); i++) {
            if (s.top() == nums[i]) {
                continue;
            }
            s.push(nums[i]);
        }
        int k = s.size(), t = k;  // ',' 运算符，从左至右进行运算
        while (t > 0) {
            t--;
            nums[t] = s.top();
            s.pop();
        }
        return k;
    }

    /**
     * LeetCode Official
     * 双指针解法
     */
    int removeDuplicates_lo(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) {
            return 0;
        }
        int fast = 1, slow = 1;
        while (fast < n) {
            if (nums[fast] != nums[fast - 1]) {
                nums[slow] = nums[fast];
                ++slow;
            }
            ++fast;
        }
        return slow;
    }
};

int main(int argc, char** argv) {
    Solution obj;
    vector<int> v = {0,0,1,1,1,2,2,3,3,4};
    int k = obj.removeDuplicates(v);
    for (int i = 0; i < k; i++) {
        cout << v[i] << " ";
    }
    return 0;
}