#include <string>
using namespace std;


class Solution {
public:
    bool is_covered(int cnt_s[], int cnt_t[]) {
        for (int i = 'A'; i <= 'Z'; i++) {
            if (cnt_s[i] < cnt_t[i]) {
                return false;
            }
        }
        for (int i = 'a'; i <= 'z'; i++) {
            if (cnt_s[i] < cnt_t[i]) {
                return false;
            }
        }
        return true;
    }
    /**
     * @author 灵神
     */
    string minWindow(string s, string t) {
        int m = s.length();
        int ans_left = -1, ans_right = m;
        int cnt_t[128]{}, cnt_s[128]{};
        for(char c : t) {
            cnt_t[c]++;
        }
        int left = 0;
        for (int right = 0; right < m; ++right) {
            cnt_s[s[right]]++;
            while (is_covered(cnt_s, cnt_t)) {
                if (right - left < ans_right - ans_left) {
                    ans_left = left;
                    ans_right = right;
                }
                cnt_s[s[left++]]--;
            }
        }
        return ans_left < 0 ? "" : s.substr(ans_left, ans_right - ans_left + 1);
    }
};