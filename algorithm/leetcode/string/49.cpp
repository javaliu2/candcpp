#include <string.h>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
   public:
   /**
    * LeetCode Official Solution(2)
    */
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        // 自定义对 array<int, 26> 类型的哈希函数
        auto arrayHash = [fn = hash<int>{}] (const array<int, 26>& arr) -> size_t {
            return accumulate(arr.begin(), arr.end(), 0u, [&](size_t acc, int num) {
                return (acc << 1) ^ fn(num);
            });
        };
        // decltype: 类型推导
        unordered_map<array<int, 26>, vector<string>, decltype(arrayHash)> mp(0, arrayHash);
        for (string& str: strs) {
            array<int, 26> counts{};  // {}: 初始化counts均为0
            int length = str.length();
            for (int i = 0; i < length; ++i) {
                counts[str[i] - 'a']++;
            }
            mp[counts].emplace_back(str);
        }
        vector<vector<string>> ans;
        for (auto it = mp.begin(); it != mp.end(); ++it) {
            ans.emplace_back(it->second);
        }
        return ans;
    }
   /**
    * LeetCode Official Solution(1)
    */
    vector<vector<string>> groupAnagrams_s1(vector<string>& strs) {
        unordered_map<string, vector<string>> mp;
        for (string& str : strs) {  // 使用&，避免拷贝构造
            string key = str;
            sort(key.begin(), key.end());
            mp[key].emplace_back(str);  // key不存在的话，会创建一个空的vector<string>
        }
        vector<vector<string>> ans;
        // ++it效率更高，因为不用保存以及返回临时对象
        for (auto it = mp.begin(); it != mp.end(); ++it) {
            ans.emplace_back(it->second);  // 比push_back()效率更高，因为原地构造对象，避免拷贝或者移动开销
        }
        return ans;
    }
    /**
     * 通过 111/126 个测试样例，超时，O(n^2)
     */
    vector<vector<string>> groupAnagrams_my(vector<string>& strs) {
        // brute force
        int n = strs.size();
        bool vis[n];
        memset(vis, 0, sizeof(vis));
        vector<vector<string>> ans;
        if (n == 0) {
            return ans;
        }
        for (int i = 0; i < n; i++) {
            if (vis[i]) {
                continue;
            }
            vis[i] = true;
            string target = strs[i];
            vector<string> t;
            t.push_back(target);  // 加入的是副本
            sort(target.begin(), target.end());
            for (int j = i + 1; j < n; j++) {
                string str_t = strs[j];  // 拷贝构造
                // sort调用导致inplace操作
                sort(str_t.begin(), str_t.end());
                if (target == str_t) {
                    t.push_back(strs[j]);
                    vis[j] = true;
                }
            }
            ans.push_back(t);
        }
        return ans;
    }
};

int main() {
    string s = "sdf";
    cout << s << endl;  // sdf
    vector<string> v;
    vector<vector<string>> vv;
    vv.push_back(v);
    v.push_back(s);
    // cout << "vv: " << vv.front().front() << endl;  // Segmentation fault
    // (core dumped)
    // 因为加入的是拷贝，因此对v的修改不会反映到vv.front()即加入的v(拷贝值)上
    cout << "vector.front(): " << v.front() << endl;  // sdf
    sort(s.begin(), s.end());                         // 原地操作, in-place
    cout << s << endl;                                // dfs
    // 那么vector.push_back()加入的是引用还是对象的副本?
    // ANS: 副本
    cout << "after sort, vector.front(): " << v.front() << endl;  // sdf
    return 0;
}