#include <string>
using namespace std;

class Solution {
   public:
    bool is_covered(int cnt_s[], int cnt_t[]) {
        for (int i = 'a'; i <= 'z'; i++) {
            if (cnt_s[i] < cnt_t[i]) {
                return false;
            }
        }
        return true;
    }
    long long validSubstringCount(string s, string t) {
        int m = s.length();
        int cnt_t[128]{}, cnt_s[128]{};
        for (char c : t) {
            cnt_t[c]++;
        }
        int left = 0;
        long long ans = 0;
        for (int right = 0; right < m; ++right) {
            cnt_s[s[right]]++;
            while (is_covered(cnt_s, cnt_t)) {
                cnt_s[s[left++]]--;
            }
            ans += left;
        }
        return ans;
    }
};

class Solution2 {
   public:
    /**
     * @author 灵神
     */
    long long validSubstringCount(string s, string t) {
        if (s.length() < t.length()) {
            return 0;
        }

        int diff[26]{};  // t 的字母出现次数与 s 的字母出现次数之差
        for (char c : t) {
            diff[c - 'a']++;
        }

        // 统计窗口内有多少个字母的出现次数比 t 的少
        int less = 0;
        for (int d : diff) {
            if (d > 0) {
                less++;
            }
        }

        long long ans = 0;
        int left = 0;
        for (char c : s) {
            diff[c - 'a']--;
            if (diff[c - 'a'] == 0) {
                // c 移入窗口后，窗口内 c 的出现次数和 t 中的一样
                less--;
            }
            while (less == 0) {                   // 窗口符合要求
                char out_char = s[left++] - 'a';  // 准备移出窗口的字母
                if (diff[out_char] == 0) {
                    // out_char 移出窗口之前，检查出现次数，
                    // 如果窗口内 out_char 的出现次数和 t 的一样，
                    // 那么 out_char 移出窗口后，窗口内 out_char 的出现次数比 t 的少
                    less++;
                }
                diff[out_char]++;
            }
            ans += left;
        }
        return ans;
    }
};