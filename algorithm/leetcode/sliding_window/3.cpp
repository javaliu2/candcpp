#include <string>
#include <string.h>
using namespace std;


class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int hash[256];
        memset(hash, 0, sizeof(hash));  // 切记! C++ must initialize.
        int left = 0, ans = 0, i;
        for (i = 0; i < s.length(); i++) {
            hash[s[i]]++;
            while (hash[s[i]] > 1) { 
                hash[s[left]]--;
                left++;
            }
            ans = max(ans, i - left + 1);
        }
        return ans;
    }
};