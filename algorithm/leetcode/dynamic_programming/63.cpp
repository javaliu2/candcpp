#include <vector>
#include <string.h>
using namespace std;

class Solution {
public:
    int uniquePathsWithObstacles_origin(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size(), n = obstacleGrid[0].size();
        long long dp[m][n];
        memset(dp, 0, sizeof(dp));
        dp[m - 1][n - 1] = 1;
        if (obstacleGrid[m - 1][n - 1]) {
            dp[m - 1][n - 1] = 0;
        }
        // 计算顺序: 从下往上、从右往左
        for (int i = m - 1; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                if (obstacleGrid[i][j]) {
                    continue;
                }
                if (i + 1 < m && !obstacleGrid[i + 1][j]) {
                    dp[i][j] += dp[i + 1][j];
                }
                if (j + 1 < n && !obstacleGrid[i][j + 1]) {
                    dp[i][j] += dp[i][j + 1];
                }
            }
        }
        return dp[0][0];
    }
    /**
    * 滚动数组
    */
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size(), n = obstacleGrid[0].size();
        long long dp[n];
        memset(dp, 0, sizeof(dp));
        dp[n - 1] = 1;
        if (obstacleGrid[m - 1][n - 1]) {
            dp[n - 1] = 0;
        }
        // 计算顺序: 从右往左
        for (int i = m - 1; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                if (obstacleGrid[i][j]) {
                    dp[j] = 0;  // attention
                    continue;
                }
                if (j + 1 < n && !obstacleGrid[i][j + 1]) {
                    dp[j] += dp[j + 1];
                }
            }
        }
        return dp[0];
    }
};