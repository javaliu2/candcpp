#include <string>
#include <algorithm>
#include <cstring>

using namespace std;
class Solution {
public:
    bool checkStrings(string s1, string s2) {
        // 遍历字符串抽取两个字符集合，分别是奇数索引和偶数索引
        string odd_set1, odd_set2, even_set1, even_set2;
        for (int i = 0; i < s1.size(); ++i) {
            if (i % 2 == 1) {
                odd_set1 += s1[i];
                odd_set2 += s2[i];
            } else {
                even_set1 += s1[i];
                even_set2 += s2[i];
            }
        }
        // 判断s1的odd_set是否和s2的odd_set一样
        // 同理判断两个字符串的even_set
        sort(odd_set1.begin(), odd_set1.end());
        sort(odd_set2.begin(), odd_set2.end());
        sort(even_set1.begin(), even_set1.end());
        sort(even_set2.begin(), even_set2.end());
        return odd_set1==odd_set2 && even_set1==even_set2;
    }

    bool checkStrings_ling(string s1, string s2) {
        int cnt1[2][26]{}, cnt2[2][26]{};
        for (int i = 0; i < s1.length(); i++) {
            cnt1[i % 2][s1[i] - 'a']++;
            cnt2[i % 2][s2[i] - 'a']++;
        }
        return memcmp(cnt1, cnt2, sizeof(cnt1)) == 0;  // 这个视角好奇妙。
    }
};