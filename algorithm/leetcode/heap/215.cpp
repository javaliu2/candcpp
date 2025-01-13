#include <vector>
using namespace std;

class Solution {
   public:
    void myswap(vector<int>& nums, int i, int j) {
        int t = nums[i];
        nums[i] = nums[j];
        nums[j] = t;
    }
    void heapify(vector<int>& nums, int node_idx) {
        int n = nums.size(), l_idx = node_idx * 2 + 1, r_idx = node_idx * 2 + 2,
            idx = l_idx;
        if (l_idx >= n) {
            return;
        }
        if (r_idx < n) {
            if (nums[r_idx] > nums[l_idx]) {
                idx = r_idx;
            }
        }
        if (nums[idx] > nums[node_idx]) {
            myswap(nums, node_idx, idx);
            heapify(nums, idx);
        }
    }

    void pop_first(vector<int>& nums) {
        nums.front() = nums.back();
        nums.pop_back();
        heapify(nums, 0);
    }

    int findKthLargest(vector<int>& nums, int k) {
        // 大根堆
        int n = nums.size() - 1;
        for (int i = (n - 1) / 2; i >= 0; i--) {
            heapify(nums, i);
        }
        // for (int ele : nums) {
        //     cout << ele << " ";
        // }
        k--;
        while (k--) {
            pop_first(nums);
        }
        return nums[0];
    }
    void heapify_debug(vector<int>& nums, int node_idx) {
        int n = nums.size(), l_idx = node_idx * 2 + 1, r_idx = node_idx * 2 + 2,
            idx = -1;
        if (l_idx >= n) {
            return;
        }
        if (nums[l_idx] > nums[node_idx]) {
            idx = l_idx;
        }
        if (r_idx < n) {
            if (idx != -1) {  // [l_idx]>[node_idx]
                if (nums[r_idx] > nums[l_idx]) {
                    idx = r_idx;
                }
            } else {
                if (nums[r_idx] > nums[node_idx]) {
                    idx = r_idx;
                }
            }
            // if (idx != -1 && nums[r_idx] > nums[l_idx]) {
            //     idx = r_idx;

            // 这里就不应该再if了，因为如果上面条件判断失败，即左存在大于node并且右没有大于左，那么这里右大于node的话，
            // 也不应该修改idx的值，因为存在一种情况，左 > 右 > node，所以说这里就不需要再判断了
            // 主要原因是因为上面有两个条件，进入下面else if的时候，我们想要的是仅仅是idx!=-1，但是第二个条件为false
            // 的话，也会进入。这是不对的。
            // } else if (nums[r_idx] > nums[node_idx]) {
            //     idx = r_idx;
            // }
        }
        if (idx == -1) {
            return;
        }
        myswap(nums, node_idx, idx);
        heapify(nums, idx);
    }
};