#include <map>
#include <vector>
using namespace std;

class Solution {
   public:
    const int MOD = 1'000'000'007;
    const vector<vector<int>> K = {
        {4, 6},    {6, 8}, {7, 9}, {4, 8}, {0, 3, 9}, {},
        {0, 1, 7}, {2, 6}, {1, 3}, {2, 4}};  // [[error 1]: 4和6包括0，因为可以横1竖2
    map<vector<int>, int> cnt;  // [[overtime 3]，因为查找pair<n, x>是否存在需要O(n)
    int cntarr[5000][10];  // 数组的话，O(1)
    int knightDialer(int n) {
        // 我一开始的思路是模拟，这显然是impossible的
        // 正确思路：dynamic programming
        // 原问题: f(n, x): 在x单元格上移动n步的方案数
        // 子问题: f(n-1, k_i):
        // x可以移动到的单元格集合为{k_i},从k_i出发，移动n-1步的方案数 边界:
        // f(0, x)为1，因为移动0步的方案数就是自己
        // 注意：knight处于单元格5时，方案数为0

        // mathematical formula:
        // dfs(n, x) = sigma_{k_i} dfs(n-1, k_i)
        long long res = 0;
        for (int x = 0; x <= 9; x++) {
            // res += dfs(n, x);  // [[error 2]: NOT n
            res += dfs(n - 1, x);
        }
        return res % MOD;
    }

    int dfs(int n, int x) {
        // check cnt if pair<n, x> exist
        // vector<int> pair = {n, x};
        // if (cnt.find(pair) != cnt.end()) {
        //     return cnt[pair];
        // }
        // int res = 0;
        int& res = cntarr[n][x];  // 直接在原始值上修改，就不用赋值了
        if (res) {
            return res;
        }
        if (n == 0) {
            return 1;
        }
        // 当前单元格x可以到达的单元格集合{k_i}
        for (int k : K[x]) {
            res = (dfs(n - 1, k) + res) % MOD;
        }
        // record result
        // cnt[pair] = res;
        return res;
    }

    int dp(int m) {
        int res = 0;
        int dp[5000][10];
        // 1 initialization
        for (int j = 0; j < 10; j++) {
            dp[0][j] = 1;
        }
        // 2 iteration
        for (int n = 1; n <= 5000; n++) {
            for (int x = 0; x < 10; x++) {
                for (int k : K[x]) {
                    dp[n][x] = (dp[n][x] + dp[n - 1][k]) % MOD;
                }
            }
            if (n == m) {
                for (int j = 0; j < 10; j++) {
                    res = (res + dp[n][j]) % MOD;
                }
                break;
            }
        }
        return res;
    }
};