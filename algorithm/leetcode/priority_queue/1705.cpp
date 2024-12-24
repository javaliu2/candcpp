#include <map>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;

// first: 苹果到期的天; second: 该天到期的苹果数量
typedef pair<int, int> pii;
/**
 * @author LeetCode Official
 */
int eatenApples(vector<int>& apples, vector<int>& days) {
    int ans = 0;
    priority_queue<pii, vector<pii>, greater<pii>> pq;  // 按照pair.first升序排列, 即每次选择最早腐烂的苹果吃
    int n = apples.size();
    int i = 0;
    while (i < n) {
        // 删除到期的苹果
        while (!pq.empty() && pq.top().first <= i) {
            pq.pop();
        }
        // 将当前遍历到的苹果加入pq
        int rottenDay = i + days[i];
        int count = apples[i];
        if (count > 0) {
            pq.emplace(rottenDay, count);
        }
        // pq非空，吃最早腐烂的苹果
        if (!pq.empty()) {
            pii curr = pq.top();
            pq.pop();
            curr.second--;
            if (curr.second != 0) {
                pq.emplace(curr.first,  curr.second);
            }
            ans++;  // 吃到的苹果数量加一
        }
        i++;
    }
    // pq非空的话继续吃
    while (!pq.empty()) {
        while (!pq.empty() && pq.top().first <= i) {
            pq.pop();
        }
        if (pq.empty()) {
            break;
        }
        pii curr = pq.top();
        pq.pop();
        // curr.first - i: 存活的天数(x), curr.second: 苹果的数量
        // 不一定就是说有x天，就一定有x个苹果，所以取两者较小值
        int num = min(curr.first - i, curr.second);
        ans += num;
        i += num;  // attention，这里加的是这么些天，而不是1天，注意与i<n时的逻辑区分
    }
    return ans;
}
/**
 * @author xs
 * 2 hours, o(╥﹏╥)o, 66 / 73 个通过的测试用例, 超出时间限制
 * 这应该是选取的数据结构不合理，或者我的算法策略不够全面
 */
int eatenApples(vector<int>& apples, vector<int>& days) {
    int ans = 0;
    int n = apples.size();
    map<int, int> remain;  // 树上的苹果数量
    map<int, int>::iterator p;
    for (int i = 0; i < n || !remain.empty(); i++) {
        if (i < n && apples[i] > 0) {
            // key不存在，value置0然后加上新的value; key存在，在原来value上新增
            remain[i + days[i]] += apples[i];
        }
        // 更新树上的苹果
        p = remain.begin();
        while (p != remain.end()) {
            if (i >= p->first) {
                p = remain.erase(p);
            } else {
                p++;
            }
        }
        cout << "[";
        for (auto& entry : remain) {
            cout << "{" << entry.first << ": " << entry.second << "}, ";
        }
        cout << "]\n";
        // 吃一个苹果，选择生存时间最短的去吃
        if (!remain.empty()) {
            p = remain.begin();
            if (p->second - 1 > 0) {
                remain[p->first] -= 1;
            } else {
                remain.erase(p);
            }
            ans++;
        }
    }
    // cout << "ans: " << ans << endl;
    // cout << "[";
    //     for (auto& entry : remain) {
    //         cout << "{" << entry.first << ": " << entry.second << "}, ";
    //     }
    //     cout << "]\n";
    // map<int, int>::reverse_iterator rp = remain.rbegin();
    // if (rp != remain.rend()) ans += min(rp->first - n, rp->second);
    return ans;
}
int main() { 
    vector<int> apples = {1,2,3,5,2};    
    vector<int> days = {3,2,1,4,2};   
    int ans = eatenApples(apples, days);
    cout << ans << endl;
    return 0; 
}