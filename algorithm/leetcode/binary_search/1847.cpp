#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;
int my_binary_search(vector<vector<int>>& rooms, int target, int preId) {
    int left = 0, right = rooms.size() - 1, idx = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (rooms[mid][1] < target) {
            left = mid + 1;
        } else {
            idx = mid;
            right = mid - 1;
        }
    }
    if (idx == -1) {
        return -1;
    }
    int value = 100'000'000, id, t;
    for (; idx < rooms.size(); idx++) {
        t = abs(rooms[idx][0] - preId);
        if (t < value) {
            value = t;
            id = idx;
        } else if (t == value) {
            if (rooms[idx][0] < rooms[id][0]) {
                id = rooms[idx][0];
            }
        }
    }
    return rooms[id][0];
}
vector<int> closestRoom(vector<vector<int>>& rooms,
                        vector<vector<int>>& queries) {
    vector<int> ans;
    // 1 sort rooms
    // sort rule: increase size, if size equals, increase roomId
    sort(rooms.begin(), rooms.end(),
         [](const vector<int>& a, const vector<int>& b) {
             if (a[1] == b[1]) {
                 return a[0] < b[0];
             }
             return a[1] < b[1];
         });
    for (const auto& room : rooms) {
        cout << "roomId: " << room[0] << ", size: " << room[1] << endl;
    }
    // 2 for(int[] q : queries)
    // binary search >= minSize的index
    // 遍历>=index的所有房间，找到abs(Id,preId)最小的房间Id
    for (vector<int> q : queries) {
        int preId = q[0], minSize = q[1];
        int roomId = my_binary_search(rooms, minSize, preId);
        ans.push_back(roomId);
    }
    return ans;
}

int main() {
    vector<vector<int>> rooms = {{7,14},{11,6},{3,1},{9,4},{14,14},{17,11},{22,13},{6,25},{12,22},{21,9}};
    vector<vector<int>> queries = {{9, 22}};
    closestRoom(rooms, queries);
    return 0;
}