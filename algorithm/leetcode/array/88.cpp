#include <vector>

using namespace std;
class Solution {
public:
    void merge_fail(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int p1 = 0, p2 = 0;
        while (p1 < m && p2 < n) {
            // 当前数组1元素为nums1[p1]，记为target
            // 将数组2中<=target的全部元素移动到nums1中
            // 将nums1中的元素移动到num2中
            // 算法题，写起来越来越烧脑，问题一定是，思路有问题。
            int target = nums1[p1];
            p2 = 0;
            while (p2 < n && nums2[p2] <= target) {
                ++p2;
            }
            // [0, p2)
            if (p2 > 0) {
                int i = p1, j = 0;
                while (j < p2) {
                    int t = nums1[i];
                    nums1[i] = nums2[j];
                    nums2[j] = t;
                    ++j;
                    ++i;
                }
            }
            if (p2 > 0) {
                p1 += p2;
            } else {
                ++p1;
            }
        }
    }

    /**
    * 听网友的，倒序遍历两个数组，比较元素大小，将大的置于nums1末尾
    */
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) { 
        int current = m + n - 1;
        int p1 = m - 1, p2 = n - 1;
        while (p1 >= 0 && p2 >= 0) {
            if (nums1[p1] <= nums2[p2]) {
                nums1[current] = nums2[p2];
                --p2;
            } else {
                nums1[current] = nums1[p1];
                --p1;
            }
            --current;
        }
        while (p2 >= 0) {
            nums1[current--] = nums2[p2--];
        }
        // 这里不需要处理p1了，因为nums1已经是有序的了
    }
};