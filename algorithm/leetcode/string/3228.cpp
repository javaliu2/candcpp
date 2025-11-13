#include <string>
using namespace std;

class Solution {
private:
    void myswap(string& s, int i, int j) {
        char t = s[i];
        s[i] = s[j];
        s[j] = t;
    }
public:
    /**
     * passed 679 / 694 test case, overtime
     */
    int maxOperations(string s) {
        int cnt = 0;
        // 模拟移动，从前往后寻找符合要求的10，找到后进行移动
        for (int i = 0; i + 1 < s.size(); ) {
            if (s[i] == '1' && s[i+1] == '0') {
                cnt++;
                int j = i + 1;
                while (j < s.size() && s[j] == '0') {
                    myswap(s, j-1, j);
                    j++;
                }
                i = i - 1 >= 0 ? i - 1 : 0;
            } else {
                    ++i;
            }
        }
        return cnt;
    }
    /**
     * @author 灵神
     * 太妙了. so wonderful.
     */
    int maxOperations_ls(string s) {
        int ans = 0, cnt1 = 0;
        for (int i = 0; i + 1 < s.size(); ++i) {
            if (s[i] == '1') {
                cnt1++;
            } else if (i > 0 && s[i-1] == '1') {
                ans += cnt1;
            }
        }
        return ans;
    }
};