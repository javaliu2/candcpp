#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class Solution {
public:
    long long maximumHappinessSum(vector<int>& happiness, int k) {
        // 贪心：循环k次，每次选择happiness中最大的幸福值，其他幸福值减1
        long long ans = 0;
        while (k--) {
            int max_v = -1, max_idx = -1;
            for (int i = 0; i < happiness.size(); ++i) {
                int v = happiness[i];
                if (v > max_v) {
                    max_v = v;
                    max_idx = i;
                }
            }
            ans += max_v;
            for (int i = 0; i < happiness.size(); ++i) {
                if (i == max_idx) {  // 如果最大值有多个，那么需要使用选取的最大值的索引来标识
                    happiness[i] = -2;
                } else if (happiness[i] > 0) {  // 等于0的幸福值不予处理
                    happiness[i] -= 1;
                }
            }
        }
        return ans;
    }
    /**
     * @author 灵神
     */
    long long maximumHappinessSum_ling(vector<int>& happiness, int k) {
        sort(happiness.begin(), happiness.end(), greater<>());  // 降序
        long long ans = 0;
        // 巧妙，走到索引为i的元素位置，该元素就应该被减去i个1，也就是减去i
        // 但是需要判断减完之后其值大于0，否则表示在前i-1轮中，该位置的值早被减至小于等于0
        for (int i = 0; i < k && happiness[i] > i; ++i) {
            ans += happiness[i] - i;
        }
        return ans;
    }
};

int main() {
    Solution solution;
    vector<int> happiness = {1, 1, 1, 1};
    int k = 2;
    int ans = solution.maximumHappinessSum(happiness, k);
    cout << "ans: " << ans;
    return 0;
}