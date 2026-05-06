#include <vector>
#include <string>

using namespace std;
class Solution {
private:
    bool isVaild(vector<vector<char>>& map, int row, int col, int n) {
        // 检查列和对角线，因为行在dfs的时候row+1确保了每行只有一个queue
        // 检查列，只需要检查到row之前的就可以
        for (int i = 0; i < row; ++i) {
            if (map[i][col] == 'Q') {
                return false;
            }
        }
        int i = row, j = col;
        // 对角线，两个方向（左上和右上），分开判断（因为这两行没有逻辑上的关联）
        while (i-1 >= 0 && j-1 >= 0) {
            if (map[--i][--j] == 'Q') {
                return false;
            }
        }
        i = row, j = col;
        while (i-1 >= 0 && j+1 < n) {
            if (map[--i][++j] == 'Q') {
                return false;
            }
        }
        return true;
    }
private:
    vector<vector<string>> res;
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<char>> map(n, vector<char>(n));
        dfs(map, 0, n);
        return res;
    }
    void dfs(vector<vector<char>>& map, int row, int n) {
        if (row == n) {
            vector<string> one;
            // for (vector<char> chs : map) {
            //     string s;
            //     for (char ch : chs) {
            //         s += ch;
            //     }
            //     one.push_back(s);
            // }
            // refined
            for (auto& chs: map) {
                one.emplace_back(chs.begin(), chs.end());
            }
            res.push_back(move(one));  // move
            return;
        }
        for (int j = 0; j < n; ++j) {
            // [row][j]防止queue是否合法
            bool vaild = isVaild(map, row, j, n);
            if (!vaild) {
                continue;
            }
            map[row][j] = 'Q';
            for (int k = 0; k < j; ++k) {
                map[row][k] = '.';
            }
            for (int k = j+1; k < n;++k) {
                map[row][k] = '.';
            }
            dfs(map, row+1, n);
            map[row][j] = '.';
        }
    }
};