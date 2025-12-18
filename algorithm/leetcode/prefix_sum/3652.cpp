#include <vector>
using namespace std;
class Solution {
public:
    /**
    * brute force, over memory
    */
    long long maxProfit_bf(vector<int>& prices, vector<int>& strategy, int k) {
        int size = prices.size();
        long long ans = 0;
        for (int i = 0; i < size; ++i) {
            ans += prices[i] * strategy[i];
        }
        // 枚举长度为k的区间的起点
        for (int i = 0; i + k <= size; ++i) {
            // [i, i+k/2), [i+k/2, i+k)
            vector<int> t_strategy = strategy;
            long long t_sum = 0;
            for (int j = i; j < i + k / 2; ++j) {
                t_strategy[j] = 0;
            }
            for (int j = i + k / 2; j < i + k; ++j) {
                t_strategy[j] = 1;
            }
            for (int j = 0; j < size; ++j) {
                t_sum += prices[j] * t_strategy[j];
            }
            ans = max(ans, t_sum);
        }
        return ans;
    }
    /**
    * nums: [x1, x2, x3, ... , x_n]
    * nums_prefix: [0, x1, x1+x2, x1+x2+x3, ..., x1+x2+...+x_n]
    * nums_prefix[idx]: 表示nums中前idx个元素之和
    * nums[l, r-1]元素之和等于nums_prefix[r] - nums_prefix[l]
    */
    long long maxProfit(vector<int>& prices, vector<int>& strategy, int k) {
        vector<long long> profit_prefix_sum, prices_prefix_sum;
        profit_prefix_sum.push_back(0);
        prices_prefix_sum.push_back(0);
        int n = prices.size();
        for (int i = 0; i < n; ++i) {
            profit_prefix_sum.push_back(profit_prefix_sum[i] + prices[i] * strategy[i]);
            prices_prefix_sum.push_back(prices_prefix_sum[i] + prices[i]);
        }
        long long ans = profit_prefix_sum[n];  // 前n个元素的和
        for (int i = k; i <= n; ++i) {
            long long t = profit_prefix_sum[i-k] + profit_prefix_sum[n] - profit_prefix_sum[i] + prices_prefix_sum[i] - prices_prefix_sum[i-k/2];
            ans = max(ans, t);
        }
        return ans;
    }
};