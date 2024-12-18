#include <vector>
using namespace std;

class Solution {
   public:
    int numRookCaptures(vector<vector<char>>& board) {
        int cnt = 0;
        // 1、先找到车
        int m = board.size(), n = board[0].size();
        int x, y;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'R') {
                    x = i;
                    y = j;
                    break;
                }
            }
        }
        // 2、上下左右移动，判断是否是边界或棋子，如果是卒，计数+1
        for (int i = x; i >= 0; i--) {
            if (board[i][y] == 'B') {
                break;
            }
            if (board[i][y] == 'p') {
                cnt++;
                break;
            }
        }
        for (int i = x; i < m; i++) {
            if (board[i][y] == 'B') {
                break;
            }
            if (board[i][y] == 'p') {
                cnt++;
                break;
            }
        }
        for (int j = y; j < n; j++) {
            if (board[x][j] == 'B') {
                break;
            }
            if (board[x][j] == 'p') {
                cnt++;
                break;
            }
        }
        for (int j = y; j >= 0; j--) {
            if (board[x][j] == 'B') {
                break;
            }
            if (board[x][j] == 'p') {
                cnt++;
                break;
            }
        }
        return cnt;
    }
};