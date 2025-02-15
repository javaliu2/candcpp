#include <vector>
#include <algorithm>
using namespace std;

/**
 * 问题描述: 给定nums数组，nums[i]表示袋子i中球的数量，给定maxOperations次操作，
 * 每次操作可以将某一个袋子中球分到两个袋子中去。
 * 目标: 进行至多maxOperations操作后，返回所有袋子中球的数量的最大值，同时最小化(由操作序列决定)该最大值
 * 思路: 参见https://leetcode.cn/problems/minimum-limit-of-balls-in-a-bag/solutions/2025611/dai-zi-li-zui-shao-shu-mu-de-qiu-by-leet-boay/
 * 思路转述: 将原问题转换为判定问题，即给定maxOperations次操作次数，能否使得所有袋子中球的最大数量不超过y
 * 通过二分查找这个数y，初始化下界为1，上界为max(nums[i])
 * 每个袋子要满足其球的数量<=y，需要进行的操作数量为(nums[i]-1)/y
 * 所有袋子均满足要求需要的操作数p等于Sigma_i{(nums[i]-1)/y}
 * 当p<=maxOperations，表明该y合法，那么我们可以继续寻找更小的y，即改变上界
 * 否则，该y不满足要求，我们需要寻找更大的y，即改变下界
 */
/**
 * 结束条件我不知道，看题解代码是 lower_bound <= up_bound.
 * 另外需要注意int越界，数据规模:
    1 <= nums.length <= 10^5
    1 <= maxOperations, nums[i] <= 10^9
    那么假设num[i]均为10^9，length取10^5
    p=10^5 * 10^9 / 2 数量级在10^13，超出int所能表示的最大值
    补充: int最大值2^31-1，大约2.15*10^9
    也没超，提交通过了，但是数据上溢是需要考虑的。
 */
class Solution {
public:
    int minimumSize(vector<int>& nums, int maxOperations) {
        int lower_bound = 1, up_bound = *max_element(nums.begin(), nums.end());
        int p, y, ans;
        while (lower_bound <= up_bound) {
            p = 0;
            y = (lower_bound + up_bound) / 2;
            for (int num : nums) {
                p += (num - 1) / y;
            }
            if (p <= maxOperations) {
                ans = y;
                up_bound = y - 1;
            } else {
                lower_bound = y + 1;
            }
        }
        return ans;
    }
};