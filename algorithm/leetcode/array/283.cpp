#include <vector>
using namespace std;

class Solution {
   public:
    /**
     * @author xs
     */
    void moveZeroes(vector<int>& nums) {
        int idx = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i]) {
                if (idx != i) {
                    nums[idx] = nums[i];
                    nums[i] = 0;
                }
                ++idx;
            }
        }
    }
    /**
     * @author LeetCode Official
     */
    void moveZeroes_o(vector<int>& nums) {
        int n = nums.size(), left = 0, right = 0;
        while (right < n) {
            if (nums[right]) {
                swap(nums[left], nums[right]);
                left++;
            }
            right++;
        }
    }
};