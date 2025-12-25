#include <vector>
#include <stack>
using namespace std;
class Solution {
public:
    vector<int> dailyTemperatures_bf(vector<int>& t) {
        // 逐一元素处理，找比他的值更大的元素，找得到的话，两者索引相减
        int size = t.size();
        vector<int> ans(size, 0);
        for (int i = 0; i < size; ++i) {
            for (int j = i + 1; j < size; ++j) {
                if (t[j] > t[i]) {
                    ans[i] = j - i;
                    break;
                }
            }
        }
        return ans;
    }
    /**
    * @idea leetcode official
    */
    vector<int> dailyTemperatures_my(vector<int>& t) {
        int size = t.size();
        vector<int> ans(size, 0);
        // 使用单调栈，存储元素的索引
        // 遍历元素，栈为空的话，将元素索引入栈
        // 否则，比较当前元素和栈顶元素大小，
        // 1）当前元素值小于等于栈顶元素值，将当前元素索引入栈
        // 2）当前元素值大于栈顶元素值，表示栈顶元素的下一个更高温度已经找到，弹出栈顶元素，计算索引之差，即为天数相差；继续比较当前元素值和新的栈顶元素值，直至栈为空或者当前元素值小于等于栈顶元素值；最后将当前元素索引入栈
        stack<int> s;
        for (int i = 0; i < size; ++i) {
            if (s.empty()) {
                s.push(i);
            } else {
                int e = s.top();
                if (t[i] <= t[e]) {
                    s.push(i);
                } else {
                    while (!s.empty() && t[(e=s.top())] < t[i]) {
                        ans[e] = i - e;
                        s.pop();
                    }
                    s.push(i);
                }
            }
        }
        return ans;
    }
    vector<int> dailyTemperatures_refine(vector<int>& t) {
        int size = t.size(), e = 0;
        vector<int> ans(size, 0);
        stack<int> s;
        for (int i = 0; i < size; ++i) {
            while (!s.empty() && t[(e=s.top())] < t[i]) {
                ans[e] = i - e;
                s.pop();
            }
            s.push(i);
        }
        return ans;
    }
};