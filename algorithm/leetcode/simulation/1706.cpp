#include <vector>

using namespace std;

class Solution {
public:
    vector<int> findBall(vector<vector<int>>& grid) {
        // 思路: 模拟
        // 1、grid[0][j]: 1, 即右移
        // (1) 判断是否是边缘
        //     是边缘的话，answer[j]为-1;
        //     不是边缘，但是grid[0][j+1]等于-1，即左移，构成V型，answer[j]为-1
        // (2) 不是以上两种情况的话，改变小球的位置
        //     当grid[i][j]为1的时候，小球位置变为[i+1,j+1], 
        //     重复以上判断，直到 i>=m 表明小球从底部掉出, 将answer[j]置为j+1
        // 2、grid[0][j]: -1, 即左移，改变判断的条件，思路相同
        int m = grid.size(), n = grid[0].size(), i, t;
        vector<int> ans;
        for (int j = 0; j < n; j++) {
            t = j;
            i = 0;
            while (i < m) {
                if (grid[i][t] == 1) {
                    if (t + 1 == n) {
                        ans.push_back(-1);
                        break;
                    }
                    if (grid[i][t + 1] == -1) {
                        ans.push_back(-1);
                        break;
                    }
                    i++;
                    t++;
                } else {
                    if (t - 1 < 0) {
                        ans.push_back(-1);
                        break;
                    }
                    if (grid[i][t - 1] == 1) {
                        ans.push_back(-1);
                        break;
                    }
                    t--;
                    i++;
                }
            }
            if (i == m) {
                ans.push_back(t);
            }
        }
        return ans;
    }
    /**
     * LeetCode Official
     * 写的比较unified
     */
    vector<int> findBall_lo(vector<vector<int>> &grid) {
        int n = grid[0].size();
        vector<int> ans(n);
        for (int j = 0; j < n; ++j) {
            int col = j; // 球的初始列
            for (auto &row : grid) {
                int dir = row[col];
                col += dir; // 移动球
                if (col < 0 || col == n || row[col] != dir) { // 到达侧边或 V 形
                    col = -1;
                    break;
                }
            }
            ans[j] = col; // col>=0且<n 为成功到达底部
        }
        return ans;
    }
};
