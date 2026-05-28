#include <vector>

using namespace std;
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int k = 0, n = nums.size();
        int vp = n-1;
        for (int i = n-1; i>=0; --i) {
            if (nums[i] == val) {
                int t = nums[i];
                nums[i] = nums[vp];
                nums[vp] = t;
                --vp;
            } else {
                ++k;
            }
        }
        return k;
    }

    int removeElement_offical(vector<int>& nums, int val) {
        int left = 0, right = nums.size()-1;
        // 针对测试用例，[1,2,2,2], val=2
        // 存在3次赋值，将[r]->[l]直至[l]不为val
        while (left <= right) {
            if (nums[left] == val) {
                nums[left] = nums[right];
                right--;
            } else {
                left++;
            }
        }
        return left;
    }
    /**
     * 网友的优化
     */
    int removeElement_wang(vector<int>& nums, int val) {
        int left = 0;
        int right = nums.size() - 1;
        while (left <= right) {
            if (nums[left] != val) {
                left++;
            } else if (nums[right] == val) {  // case [l]==val且[r]==val，那么直接--r，避免赋值
                right--;
            } else { // case [l]==val且[r]!=val
                nums[left] = nums[right];
                right--;
                left++;
            }
        }
        return left;
    }
};