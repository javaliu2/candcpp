#include <vector>
#include <unordered_set>
#include <cstring>

using namespace std;
class Solution {
public:
    int findDuplicate_my(vector<int>& nums) {
        unordered_set<int> set;
        for (int num : nums) {
            if (set.count(num)) {
                return num;
            }
            set.insert(num);
        }
        return -1;
    }
    /**
    * 题目：有n+1个数，每个数的取值范围在[1,n]，所以可知至少存在一个重复的数（由鸽巢原理可知）。
    * 问题，找出这个重复的数。
    * 解答思路：遍历[1,n]范围所构成的数组，即{1, 2, 3, ..., n-1, n}中每一个元素i,
    * 统计在nums中<=i的元素个数。
    * 示例1：nums: [1,3,4,2,2]
    *    i: 1, 2, 3, 4
    *  cnt: 1, 3, 4, 5
    * 假设重复元素为target，那么由以上cnt结果可知，[1,target-1]范围内元素的cnt值<=i
    * [target, n]范围内元素的cnt值>i（由于有重复元素的缘故）
    * 示例2：nums: [3,3,3,3,3], n=4
    *      i: 1, 2, 3, 4
    *    cnt: 0, 0, 5, 5
    * 此例中，target为3，[1,2]范围内元素的cnt值<=i, [3,4]范围内元素的cnt值>i
    * 那么提示到这里，按照我的思路，就是计算cnt数组，然后再去找到第一个元素i，满足其cnt值>i，该元素就是结果了。但是这样又不叫二分查找了。
    */
    // 超时，passed 54 / 59
    int findDuplicate_overtime(vector<int>& nums) {
        int n = nums.size()-1;
        int cnts[n+1];
        memset(cnts, 0, sizeof(cnts));
        for (int i = 1; i<=n; ++i) {
            int cnt = 0;
            for (int num : nums) {
                if (num <= i) {
                    cnt++;
                }
            }
            cnts[i] = cnt;
        }
        for (int i = 1; i<=n; ++i) {
            if (cnts[i] > i) {
                return i;
            }
        }
        return -1;
    }
    /**
    * official, it's wonderful. i can only say niubility.
    */
    int findDuplicate(vector<int>& nums) {
        int n = nums.size();
        // 由于cnt数组是递增的，因此通过[l,r]来找到target
        int l = 1, r = n-1, ans = -1;  // [l,r]就是题目中的[1,n]
        while (l <= r) {
            int mid = (l + r) >> 1;  // 判断当前的mid, 即解答思路中的i，是否满足条件
            int cnt = 0;  // 计算mid的cnt值，即nums中<=mid的元素个数
            for (int i = 0; i < n; ++i) {
                cnt += nums[i] <= mid;
            }
            if (cnt <= mid) {  // cnt[i]<=i, 该元素在target左侧
                l = mid + 1;  // 缩小范围
            } else {
                r = mid - 1;
                ans = mid;  // 该mid满足条件，但是有没有更小的满足条件的值，所以修改右侧边界
            }
        }
        return ans;
    }
};