#include <vector>
#include <string.h>
using namespace std;

class Solution {
public:
    bool isPossibleToSplit(vector<int>& nums) {
        // 统计每一个元素的个数，个数>=3，即fail
        int cnt[101];
        memset(cnt, 0, sizeof(cnt));
        for (int& ele : nums) {
            cnt[ele]++;
            if (cnt[ele] >= 3) {
                return false;
            }
        }
        return true;
    }
};