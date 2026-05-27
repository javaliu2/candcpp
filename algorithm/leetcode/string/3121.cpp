#include <string>
#include <vector>
#include <iostream>
using namespace std;
class Solution {
public:
    int numberOfSpecialChars(std::string word) {
        // 以下写法只会把第一个元素初始化-1，其他为0
        // int lower_char_last_idx[26] = {-1};
        // int upper_char_first_idx[26] = {-1};
        vector<int> lower_char_last_idx(26, -1);
        vector<int> upper_char_first_idx(26, -1);
        int count = 0;
        for (int i = 0; i < word.size(); ++i) {
            char ch = word[i];
            if (ch >= 'a' && ch <= 'z') {
                lower_char_last_idx[ch - 'a'] = i;
            } else {
                int idx = ch - 'A';
                if (upper_char_first_idx[idx] == -1) {
                    upper_char_first_idx[idx] = i;
                }
            }
        }
        for (int i = 0; i < 26; ++i) {
            if (lower_char_last_idx[i] != -1 && lower_char_last_idx[i] < upper_char_first_idx[i]) {
                ++count;
            }
        }
        return count;
    }
};

int main() {
    Solution sol;
    std::string s = "AbBCab";  // 针对该用例，c没有出现，其索引值为默认值-1，只出现了C，判断结果为true，但是是不对的，因为没有考虑小写字母不存在的情况
    int res = sol.numberOfSpecialChars(s);
    cout << res << endl;
    return 0;
}