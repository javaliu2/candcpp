#include <vector>

using namespace std;

class Solution {
public:
    vector<vector<char>> rotateTheBox(vector<vector<char>>& boxGrid) {
        int m = boxGrid.size(), n = boxGrid[0].size();
        vector<vector<char>> res(n, vector<char>(m));
        for (int i = 0; i < m; ++i) {
            for (int j = n-1; j >= 0; --j) {
                char ch = boxGrid[i][j];
                // res中的旋转坐标为[j][m-1-i]
                if (ch == '.' || ch == '*') {
                    res[j][m-1-i] = ch;
                } else {  // '#'
                    // 下落直至遇到石头/障碍物/箱子底部
                    int t = j;
                    while (t + 1 < n && boxGrid[i][t+1] == '.') {
                        ++t;
                    }
                    // res中的下落坐标为[t][m-1-i]
                    res[t][m-1-i] = ch;
                    if (t != j) {
                        res[j][m-1-i] = '.';
                        boxGrid[i][j] = '.';
                        boxGrid[i][t] = '#';
                    }
                }
            }
        }
        return res;
    }
};