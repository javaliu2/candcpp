#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        // 从后往前寻找正序对 <j-1, j> 满足 nums[j-1] < nums[j]
        // 找到之后，找[j, len)范围内所有元素中 满足 大于nums[j-1]的最小元素min_ele
        // 将min_ele置于[j-1]的位置，将[j-1]原本的元素和[j, len)剩下的元素升序排列
        int j = -1, len = nums.size();
        for (int i = len-1; i > 0; --i) {
            if (nums[i-1] < nums[i]) {
                j = i;
                break;
            }
        }
        if (j != -1) {
            int k = j + 1, j_1 = nums[j-1], min_idx = j;
            while (k < len) {
                if (nums[k] > j_1) {
                    if (nums[k] < nums[min_idx]) {
                        min_idx = k;
                    }
                }
                ++k;
            }
            swap(nums[j-1], nums[min_idx]);
            // sort [j,len)
            sort(nums.begin() + j, nums.end());
        } else {
            sort(nums.begin(), nums.end());
        }
    }
};

int main() {
    vector<int> nums{1,3,2};
    Solution sol;
    sol.nextPermutation(nums);
    return 0;
}