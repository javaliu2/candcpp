#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    /**
     * @author: 灵茶山艾府
     * 思路见https://leetcode.cn/problems/minimum-cost-for-cutting-cake-ii/
    */
    long long minimumCost(int m, int n, vector<int>& horizontalCut, vector<int>& verticalCut) {
        sort(horizontalCut.begin(), horizontalCut.end());
        sort(verticalCut.begin(), verticalCut.end());
        long long ans = 0;
        int i = 0, j = 0;
        while (i < m - 1 || j < n - 1) {
            if (j == n - 1 || i < m - 1 && horizontalCut[i] < verticalCut[j]) {
                ans += horizontalCut[i++] * (n - j); // 上下连边
            } else {
                ans += verticalCut[j++] * (m - i); // 左右连边
            }
        }
        return ans;
    }

    // brute force
    int minimumCost_my(int m, int n, vector<int>& h, vector<int>& v) {
        return dfs(0, 0, m - 1, n - 1, h, v);
    }
    /**
        lu_r: left upper row; c: column
        rd_r: right down row; c: column
        这四个参数唯一确定一个子问题
    */
    int dfs(int lu_r, int lu_c, int rd_r, int rd_c, vector<int>& h, vector<int>& v) {
        // 显式定义出口
        if (lu_r == rd_r && lu_c == rd_c) {
            return 0;
        }
        // 针对该子问题，计算所有方案的开销，返回最小开销
        // 每次只能切割行 或者 列
        // case 1: 切割行
        // int cost_1 = 0, cost_2 = 0;
        int res = 65535;
        for (int i = lu_r; i < rd_r; i++) {
            // 分解为两个子问题, 递归调用dfs解决这两个子问题，开销等于子问题的开销加上本次切割的开销
            int cost1 = dfs(lu_r, lu_c, i, rd_c, h, v);
            int cost2 = dfs(i + 1, lu_c, rd_r, rd_c, h, v);
            // cost_1 = cost1 + cost2 + (rd_c - lu_c + 1) * h[i];  // 这里写的有问题, 不是乘起来啊，一次切割只有一个数值的开销
            res = min(res, cost1 + cost2 + h[i]);
        }
        // case 2: 切割列
        for (int j = lu_c; j < rd_c; j++) {
            int cost1 = dfs(lu_r, lu_c, rd_r, j, h, v);
            int cost2 = dfs(lu_r, j + 1, rd_r, rd_c, h, v);
            // cost_2 = cost1 + cost2 + (rd_r - lu_r + 1) * v[j];
            res = min(res, cost1 + cost2 + v[j]);
        }
        // 返回以上两个case开销的较小值
        // return min(cost_1, cost_2);  这个逻辑也不对
        return res;
    }
};