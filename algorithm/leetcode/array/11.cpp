#include <vector>
using namespace std;

class Solution {
public:
    int maxArea_bf(vector<int>& height) {
        // brute force, 58/65个通过的测试用例
        int _max = -1;
        for (int i = 0; i < height.size(); ++i) {
            for (int j = i + 1; j < height.size(); ++j) {
                int area = (j - i) * min(height[i], height[j]);
                _max = max(_max, area);
            }
        }
        return _max;
    }

    /**
     * @idea LeetCode Official
     */
    int maxArea(vector<int>& height) {
        int l = 0, r = height.size() - 1;
        int ans = 0;
        while (l != r) {
            if (height[l] <= height[r]) {
                ans = max(ans, height[l] * (r - l));
                l++;
            } else {
                ans = max(ans, height[r] * (r - l));
                r--;
            }
        }
        return ans;
    }
};