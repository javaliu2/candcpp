#include <queue>

using namespace std;

class Solution {
public:
    int minOperations(vector<int>& nums, int k) {
        int cnt = 0;
        // priority_queue是容器适配器，第二个参数指定其借助的容器，第三个参数指定是大根堆还是小根堆
        // attention overflow of int
        priority_queue<long long, vector<long long>, greater<long long>> pq;
        for (int num : nums) {
            pq.push(num);
        }
        long long min_num = pq.top(), min_num2;
        while (min_num < k) {
            pq.pop();
            min_num2 = pq.top(); pq.pop();
            // pq.push(min(min_num, min_num2) * 2 + max(min_num, min_num2));
            pq.push(min_num * 2 + min_num2);  // min_num一定小于min_num2
            min_num = pq.top();
            cnt++;
        }
        return cnt;
    }
};