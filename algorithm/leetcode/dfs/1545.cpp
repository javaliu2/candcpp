#include <string>
#include <algorithm>
#include <cmath>
using namespace std;

class Solution {
private:
    string invert(string s) {
        for (int i = 0; i < s.size(); ++i) {
            if (s[i] == '0') {
                s[i] = '1';
            } else {
                s[i] = '0';
            }
        }
        return s;
    }
    string dfs(int n) {
        if (n == 1) {
            return "0";
        }
        string s = dfs(n-1);
        string s_t = invert(s);
        std::reverse(s_t.begin(), s_t.end());
        return s + "1" + s_t;
    }
    /**
    * @idea leetcode official
    */
    char dfs2(int n, int k) {
        if (k == 1) {
            return '0';
        }
        int len_n_1 = (int)pow(2, n-1);
        int len_n = (int)pow(2, n);
        if (k == len_n_1) {
            return '1';
        } else if (k < len_n_1) {
            return dfs2(n-1, k);
        }
        char c = dfs2(n-1, len_n - k);
        return c == '1' ? '0' : '1';
    }
public:
    char findKthBit(int n, int k) {
        // cout << dfs(n);
        // string s = dfs(n);
        // return s[k-1];
        return dfs2(n, k);
    }
    /**
     * 官方代码，比较清晰
     * 思路介绍: https://leetcode.cn/problems/find-kth-bit-in-nth-binary-string/solutions/382517/zhao-chu-di-n-ge-er-jin-zhi-zi-fu-chuan-zhong-de-2/?envType=daily-question&envId=2026-03-03
     */
    char findKthBit2(int n, int k) {
        if (k == 1) {
            return '0';
        }
        int mid = 1 << (n - 1);  // 2^(n-1)-1是S_(n-1)的长度t，t+1是'1'的位置（从1数起）
        if (k == mid) {
            return '1';
        } else if (k < mid) {   // 第k个字符在前t个字符中
            return findKthBit2(n - 1, k);
        } else {
            k = mid * 2 - k;   // 第k个字符在S_n的第(2^n-k)个字符处
            return invert(findKthBit2(n - 1, k));
        }
    }

    char invert(char bit) {
        return (char) ('0' + '1' - bit);
    }
};