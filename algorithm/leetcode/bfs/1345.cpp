#include <vector>
#include <iostream>
#include <queue>
#include <unordered_map>

using namespace std;
class Solution {
public:
    // over memory, passed 22/33. 第一次碰到超出内存限制的，一般都是overtime
    int minJumps(vector<int>& arr) {
        int n = arr.size();
        // 1) set up edge
        vector<vector<int>> edge(n, vector<int>(n, 0));
        for (int i = 0; i < n; ++i) {
            if (i - 1 >= 0) {
                edge[i][i - 1] = 1;
            }
            if (i + 1 < n) {
                edge[i][i + 1] = 1;
            }
            for(int j = 0; j < n; ++j) {
                if (i == j) {
                    continue;
                }
                if (arr[i] == arr[j]) {
                    edge[i][j] = 1;
                }
            }
        }
        // for (auto arr1 : edge) {
        //     for (auto ele : arr1) {
        //         std::cout << ele << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // 2) using bfs finding shortest path
        std::queue<int> q;
        q.push(0);
        int distance = 0;
        std::vector<bool> visited(n, false);
        std::vector<bool> shortest_ok(n, false);  // 该元素已经找到最短路径
        while (!q.empty()) {
            int size = q.size();
            bool finded = false;
            for (int i = 0; i < size; ++i) {
                int node_idx = q.front();
                if (node_idx == n-1) {
                    finded=true;  // CAO! 犯了一个低级错误，这行代码放到break后边了
                    break;
                }
                q.pop();
                visited[node_idx] = true;
                for (int j = 0; j < n; ++j) {
                    if (!visited[j] && edge[node_idx][j] == 1 && !shortest_ok[j]) {
                        shortest_ok[j] = true;
                        q.push(j);
                    }
                }
            }
            if (finded) {
               break; 
            }
            ++distance;
        }
        return distance;
    }

    int minJumps_copilot(vector<int>& arr) {
        int n = arr.size();
        if (n <= 1) return 0;

        unordered_map<int, vector<int>> positions;
        for (int i = 0; i < n; ++i) {
            positions[arr[i]].push_back(i);
        }

        std::queue<int> q;
        q.push(0);
        std::vector<bool> visited(n, false);
        visited[0] = true;
        int distance = 0;

        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                int idx = q.front();
                q.pop();
                if (idx == n - 1) {
                    return distance;
                }

                int left = idx - 1;
                if (left >= 0 && !visited[left]) {
                    visited[left] = true;
                    q.push(left);
                }

                int right = idx + 1;
                if (right < n && !visited[right]) {
                    visited[right] = true;
                    q.push(right);
                }

                auto &same = positions[arr[idx]];
                for (int next : same) {
                    if (!visited[next]) {
                        visited[next] = true;
                        q.push(next);
                    }
                }
                same.clear(); // 防止重复遍历相同值的索引
            }
            ++distance;
        }
        return distance;
    }
};

int main() {
    Solution s;
    std::vector<int> arr{100,-23,-23,404,100,23,23,23,3,404};
    std::cout << s.minJumps(arr) << std::endl;
    return 0;
}