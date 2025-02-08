#include <bits/stdc++.h>
using namespace std;


/**
 * 缝缝补补，喵的，写的像屎一样。
 */
class Solution {
public:
    /**
    * 判断窗口window内所有字母是否符合预期
    * target 和 window 键一样，且值相等
    */
    bool isValid(const unordered_map<char, int>& target, const unordered_map<char, int>& window) {
        for (const auto& pair : window) {
            // auto it = target.find(pair.first);
            // if (it == target.end() || it->second != pair.second) {
            //     return false;
            // }
            if (!target.count(pair.first)) {
                return false;
            }
            // target[pair.first]报错: no viable overloaded operator[] 
            // for type 'const unordered_map<char, int>', 因为target被const修饰，
            // operator[]没有const版本的实现
            if (pair.second != target.at(pair.first)) {  // 使用 at() 访问
                return false;
            }
        }
        return true;
    }
    vector<int> findAnagrams(string s, string p) {
        vector<int> ans;
        // 如果p中字母个数多于s中的，那么一定不存在异位词
        if (p.length() > s.length()) {
            return ans;
        }
        // 使用unordered_map记录目标p中每个字母的出现次数
        unordered_map<char, int> cnt_target, cnt_window;
        for (char ch : p) {
            cnt_target[ch]++;
        }
        int length_of_window = p.length(), i;
        /**
        * 子串，要求连续，因此本题是固定长度窗口
        * 使用unordered_map记录该窗口内字母出现次数
        */
        // 初始化
        for (int j = 0; j < length_of_window; j++) {
            cnt_window[s[j]]++;
        }
        // i枚举窗口起始索引
        for (i = 0; i + length_of_window < s.length(); i++) {
            // 判断cnt_window是否合法
            if (isValid(cnt_target, cnt_window)) {
                ans.push_back(i);
            }
            // 滑动窗口
            cnt_window[s[i]]--;
            cnt_window[s[i + length_of_window]]++;
            if (cnt_window[s[i]] == 0) {
                cnt_window.erase(s[i]);
            }
        }
        if (isValid(cnt_target, cnt_window)) {
            ans.push_back(i);
        }
        return ans;
    }

    /**
     * LeetCode Official
     * 看人家写的，真简洁
     */
    vector<int> findAnagrams_lo(string s, string p) {
        int sLen = s.size(), pLen = p.size();

        if (sLen < pLen) {
            return vector<int>();
        }

        vector<int> ans;
        vector<int> sCount(26);
        vector<int> pCount(26);
        for (int i = 0; i < pLen; ++i) {
            ++sCount[s[i] - 'a'];
            ++pCount[p[i] - 'a'];
        }

        if (sCount == pCount) {  // operator overload
            ans.emplace_back(0);
        }

        for (int i = 0; i < sLen - pLen; ++i) {
            --sCount[s[i] - 'a'];
            ++sCount[s[i + pLen] - 'a'];

            if (sCount == pCount) {
                ans.emplace_back(i + 1);
            }
        }

        return ans;
    }

};

int main(int argc, char** argv) {
    Solution obj;
    string s = "baa", p = "aa";
    vector<int> ans = obj.findAnagrams(s, p);
    for (int v : ans) {
        cout << v << " ";
    }
    vector<int> v1 = {1, 2, 3};
    vector<int> v2 = {1, 2};
    cout << (v1 == v2);
    return 0;
}