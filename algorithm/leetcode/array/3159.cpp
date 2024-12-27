#include <vector>
#include <string.h>
using namespace std;

class Solution {
public:
    /**
     * @author 灵神
     */
    vector<int> occurrencesOfElement_ling(vector<int>& nums, vector<int>& queries, int x) {
        vector<int> pos;
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] == x) {
                pos.push_back(i);
            }
        }
        for (int& q : queries) {
            q = q > pos.size() ? -1 : pos[q - 1];
        }
        return queries;
    }

    vector<int> occurrencesOfElement(vector<int>& nums, vector<int>& queries, int x) {
        // 遍历nums, 记录x每次出现的下标
        int n = nums.size();
        int record[n + 1];
        memset(record, -1, sizeof(record));
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            if (nums[i] == x) {
                record[++cnt] = i;
            }
        }
        // 遍历queries, 查询record, 将该出现次数的下标加入答案，如果不存在加入-1
        vector<int> ans;
        for (int& query : queries) {
            if (query > n) {
                ans.push_back(-1);
                continue;
            }
            ans.push_back(record[query]);
        }
        return ans;
    }
};