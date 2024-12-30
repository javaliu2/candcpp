#include <vector>
using namespace std;

class Solution {
public:
    int minimumOperations(vector<vector<int>>& grid) {
        int ans = 0;
        for (int j = 0; j < grid[0].size(); j++) {
            for (int i = 1; i < grid.size(); i++) {
                if (grid[i][j] <= grid[i - 1][j]) {
                    int t = grid[i - 1][j] - grid[i][j] + 1;
                    ans += t;
                    grid[i][j] += t;
                }
            }
        }
        return ans;
    }
};