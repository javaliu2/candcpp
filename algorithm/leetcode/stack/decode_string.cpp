#include <string>
#include <stack>
#include <iostream>
#include <algorithm>
using namespace std;
/**
 * 给定一个经过编码的字符串，返回它解码后的字符串。
 * 编码规则为k[encoded_string]，表示方括号内部的encoded_string正好重复k次，注意k保证为正整数。
 * 示例：
 * 1) 3[ab]4[s] => abababssss
 * 2) 2[3[sd]2[as4[qq]]]qwe => sdsdsdasqqqqqqqqasqqqqqqqqsdsdsdasqqqqqqqqasqqqqqqqqqwe
 */
string func(string s) {
    /**
     * 遍历字符串s，判断当前字符c是：
     * 1）数字，由于数字可能是多位的，那么就寻找[，从而计算出k. 然后将k入数字栈
     * 2）[，将[入字符栈
     * 3) ]，从字符栈pop出字符，逆序排列，构成字符串s_tmp，直到pop出[，从数字栈pop出一个数字k_tmp，将s_tmp重复k_tmp次
     * 得到的字符串追加到结果字符串s_ans上
     * 4）字符，将字符追加到s_ans上
     */
    stack<uint32_t> num_stack;
    stack<char> char_stack;
    int n = s.size();
    for (int i = 0; i < n; ) {
        char c = s[i];
        if (c >= '0' && c <= '9') {
            int j = i, k = 0;
            while (j < n && s[j] != '[') {
                k = k * 10 + (s[j] - '0');
                j++;
            }
            num_stack.push(k);
            // 这里也有bug，如果是多位数，下次循环i不能是++，而是j
            i = j;
            continue;
        } else if (c == '[') {
            char_stack.push(c);
        } else if (c == ']') {
            string s_tmp;
            while (!char_stack.empty() && char_stack.top() != '[') {
                s_tmp += char_stack.top();
                char_stack.pop();
            }
            char_stack.pop();
            // s_tmp.reserve();  // 这是预留函数而不是翻转
            reverse(s_tmp.begin(), s_tmp.end());
            int k_tmp = num_stack.top();
            num_stack.pop();
            string s_in;
            s_in.reserve(s_tmp.size() * k_tmp);  // reserve的正确使用姿势，避免多次realloc
            while (k_tmp--) {
                s_in += s_tmp;
            }
            for (char c : s_in) {
                char_stack.push(c);
            }
        } else {
            char_stack.push(c);
        }
        i++;
    }
    string s_ans;
    s_ans.reserve(char_stack.size());
    while (!char_stack.empty()) {
        s_ans += char_stack.top();
        char_stack.pop();
    }
    reverse(s_ans.begin(), s_ans.end());
    return s_ans;
}
// "bcd2[3[sd]2[as4[qq]]]qwe";
string func_recursion(string& s, int& pos) {
    string ans;
    while (pos < s.size()) {
        char c = s[pos];
        if (isalpha(c)) {
            ans += c;
            ++pos;
        } else if (isdigit(c)) {
            int k = 0;
            while (pos < s.size() && s[pos] != '[') {
                k = k * 10 + (s[pos] - '0');
                ++pos;
            }
            ++pos;  // 跳过'['
            string res = func_recursion(s, pos);
            while (k--) {
                ans += res;
            }
        } else if (c == ']') {
            ++pos;  // 跳过']'
            return ans; 
        }
    }
    return ans;
}
int main(int argc, char** argv) {
    string s = "3[ab]4[s]";
   
    s = "10[a]";
    s = "3[4[bc]aa]def";
    s = "bcd2[3[sd]2[as4[qq]]]qwe";
    s = "1[3[a]5[4[sd3[as2[a]]]]]";
    auto res = func(s);
    int pos = 0;
    auto res2 = func_recursion(s, pos);
    cout << (res == res2) << endl;
    cout << res << endl;
    cout << res2 << endl;
    return 0;
}
