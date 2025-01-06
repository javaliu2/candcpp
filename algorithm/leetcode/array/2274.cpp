#include <algorithm>
#include <vector>

using namespace std;
class Solution {
   public:
   /**
    * @author xs
    */
    int maxConsecutive(int bottom, int top, vector<int>& s) {
        // brute force, O(n^2)
        int _max = -1, n = s.size();
        sort(s.begin(), s.end());
        if (n == 1) {
            _max = max(s[0] - bottom, top - s[0]);
        } else if (n == 2) {
            _max = max(s[0] - bottom, top - s[1]);
            _max = max(_max, s[1] - s[0] - 1);
        } else {
            _max = max(s[0] - bottom, top - s[n - 1]);
            for (int i = 1; i < n - 1; ++i) {
                int t = max(s[i] - s[i - 1] - 1, s[i + 1] - s[i] - 1);
                _max = max(_max, t);
            }
        }
        return _max;
    }

    /**
     * @author LeetCode Official
     */
    int maxConsecutive2(int bottom, int top, vector<int>& special) {
        // 妙啊
        special.push_back(bottom - 1);
        special.push_back(top + 1);
        sort(special.begin(), special.end());

        int n = special.size();
        int ans = 0;
        for (int i = 0; i < n - 1; ++i) {
            ans = max(ans, special[i + 1] - special[i] - 1);
        }
        return ans;
    }
};