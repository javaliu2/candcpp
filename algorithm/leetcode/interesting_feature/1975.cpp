#include <vector>
#include <stdlib.h>

using namespace std;

class Solution {
public:
    /**
    * @idea: LC Official
    */
    long long maxMatrixSum(vector<vector<int>>& matrix) {
        long long abs_sum = 0;
        int abs_min = 100010, negative_num_cnt = 0;
        int n = matrix.size();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] < 0) {
                    negative_num_cnt++;
                }
                int abs_num = abs(matrix[i][j]);
                if (abs_num < abs_min) {
                    abs_min = abs_num;
                }
                abs_sum += abs_num;
            }
        }
        if ((negative_num_cnt & 1) == 1) {
            return abs_sum - abs_min * 2;
        } 
        return abs_sum;
    }
};