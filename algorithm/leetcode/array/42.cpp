#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    /**
     * dynamic programming
     */
    int trap_dp(vector<int>& height) {
        int n = height.size(), ans = 0;
        vector<int> left_max(n), right_max(n);
        left_max[0] = height[0];
        right_max[n - 1] = height[n - 1];
        for (int i = 1; i < n; i++) {
            left_max[i] = max(left_max[i - 1], height[i]);
            right_max[n - 1 - i] = max(right_max[n - i], height[n - 1 - i]);
        }
        for (int i = 0; i < n; i++) {
            ans += min(left_max[i], right_max[i]) - height[i];
        }
        return ans;
    }
    /**
     * descending stack
     */
    int trap_ds(vector<int>& height) {
        stack<int> s;
        int ans = 0;
        for (int i = 0; i < height.size(); i++) {
            while (!s.empty() && height[s.top()] < height[i]) {
                int top_idx = s.top(), left_idx;
                s.pop();
                if (s.empty()) {
                    break;
                }
                left_idx = s.top();
                ans += (i - left_idx - 1) * (min(height[left_idx], height[i]) - height[top_idx]);
            }
            s.push(i);
        }
        return ans;
    }
};