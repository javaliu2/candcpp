#include <string>
using namespace std;

class Solution {
   public:
    /**
     * @author: 灵神
     */
    bool isSubstringPresent(string s) { 
        bool vis[26][26];
        for (int i = 1; i < s.size(); i++) {
            int x = s[i - 1] - 'a', y = s[i] - 'a';
            vis[x][y] = true;
            if (vis[y][x]) {
                return true;
            }
        }
        return false; 
    }
    bool isSubstringPresent_my(string s) {
        // brute force
        int n = s.size();
        for (int i = 0; i + 1 < n; i++) {
            // 子字符串：[i, i+1]
            for (int j = n - 1; j - 1 >= 0; j--) {
                if (s[j] == s[i] && s[j - 1] == s[i + 1]) {
                    return true;
                }
            }
        }
        return false;
    }
};