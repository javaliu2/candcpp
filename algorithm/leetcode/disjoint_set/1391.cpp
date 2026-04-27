#include <vector>
#include <ostream>
using namespace std;

class DisjointSet {
    public:
        int* f;
        int mSize;
        DisjointSet(int size) {
            f = new int[size];
            mSize = size;
            for (int i = 0; i < size; ++i) {
                f[i] = i;
            }
        }
        ~DisjointSet() {
            delete[] f;
        }
        // 返回的是该节点node所在集合根元素的索引
        int findRoot(int node) {
            if (node == f[node]) {
                return node;
            }
            return findRoot(f[node]);
            // return f[node] = findRoot(f[node]);  // 路径压缩
        }
        void unionNums(int father, int child) {
            int father_idx = findRoot(father);
            int child_idx = findRoot(child);
            if (father_idx != child_idx) {
                f[child_idx] = father_idx;
            }
        }
        friend ostream& operator<<(ostream& os, const DisjointSet& set) {
            for (int i = 0; i < set.mSize; ++i) {
                os << "i=" << i << " => f[i]="<< set.f[i] << ";";
            }
            os << "\n";
            return os;
        }
    };
class Solution {
private:
    /**
    * x表示上下，y表示左右。因为是数组坐标轴，而不是数学坐标轴，不要混淆了。
    * 二维(x,y)到一维id的映射是统一的，即id=n*x+y
    */
    bool detectL(int x, int y) {
        if (y - 1 < 0) {
            return false;
        }
        int num = mGrid[x][y-1];
        if (num == 1 || num == 4 || num == 6) {
            return true;
        }
        return false;
    }
    bool detectR(int x, int y) {
        if (y + 1 >= mN) {
            return false;
        }
        int num = mGrid[x][y + 1];
        if (num == 1 || num == 3 || num == 5) {
            return true;
        }
        return false;
    }
    bool detectU(int x, int y) {
        if (x - 1 < 0) {
            return false;
        }
        int num = mGrid[x - 1][y];
        if (num == 2 || num == 3 || num == 4) {
            return true;
        }
        return false;
    }
    bool detectB(int x, int y) {
        if (x + 1 >= mM) {
            return false;
        }
        int num = mGrid[x + 1][y];
        if (num == 2 || num == 5 || num == 6) {
            return true;
        }
        return false;
    }
private:
    vector<vector<int>> mGrid;
    int mM;
    int mN;
public:
    bool hasValidPath(vector<vector<int>>& grid) {
        mGrid = grid;
        mM = grid.size();
        mN = grid[0].size();
        
        DisjointSet disSet(mM * mN);

        auto getId = [&](int x, int y) {
            return x * mN + y;
        };

        for (int x = 0; x < mM; ++x) {
            for (int y = 0; y < mN; ++y) {
                int num = grid[x][y];
                int current_id = getId(x, y);
                switch(num) {
                    case 1: {
                        bool left_ok = detectL(x, y);
                        bool right_ok = detectR(x, y);
                        if (left_ok) {
                            // 将grid[x][y]和左侧的grid[x-1][y]连接
                            // 需要将二维索引映射为一维索引
                            int left_id = getId(x, y - 1);
                            disSet.unionNums(left_id, current_id);
                        } 
                        if (right_ok) {
                            int right_id = getId(x, y + 1);
                            disSet.unionNums(right_id, current_id);
                        }
                    } break;
                    case 2: {
                        bool upper_ok = detectU(x, y);
                        bool below_ok = detectB(x, y);
                        if (upper_ok) {
                            int upper_id = getId(x - 1, y);
                            disSet.unionNums(upper_id, current_id);
                        } 
                        if (below_ok) {
                            int below_id = getId(x + 1, y);
                            disSet.unionNums(below_id, current_id);
                        }
                    } break;
                    case 3: {
                        bool left_ok = detectL(x, y);
                        bool below_ok = detectB(x, y);
                        if (left_ok) {
                            int left_id = getId(x, y - 1);
                            disSet.unionNums(left_id, current_id);
                        } 
                        if (below_ok) {
                            int below_id = getId(x + 1, y);
                            disSet.unionNums(below_id, current_id);
                        }
                    } break;
                    case 4: {
                        bool right_ok = detectR(x, y);
                        bool below_ok = detectB(x, y);
                        if (right_ok) {
                            int right_id = getId(x, y + 1);
                            disSet.unionNums(right_id, current_id);
                        } 
                        if (below_ok) {
                            int below_id = getId(x + 1, y);
                            disSet.unionNums(below_id, current_id);
                        }
                    } break;
                    case 5: {
                        bool left_ok = detectL(x, y);
                        bool upper_ok = detectU(x, y);
                        if (left_ok) {
                            int left_id = getId(x, y - 1);
                            disSet.unionNums(left_id, current_id);
                        } 
                        if (upper_ok) {
                            int upper_id = getId(x - 1, y);
                            disSet.unionNums(upper_id, current_id);
                        }
                    } break;
                    case 6: {
                        bool upper_ok = detectU(x, y);
                        bool right_ok = detectR(x, y);
                        if (upper_ok) {
                            int upper_id = getId(x - 1, y);
                            disSet.unionNums(upper_id, current_id);
                        } 
                        if (right_ok) {
                            int right_id = getId(x, y + 1);
                            disSet.unionNums(right_id, current_id);
                        }
                    } break;
                }
            }
        }

        int _0_0_idx = disSet.findRoot(getId(0, 0));
        int _m_n_idx = disSet.findRoot(getId(mM-1, mN-1));
        if (_0_0_idx == _m_n_idx) {
            return true;
        }
        // cout << disSet;
        return false;
    }
};