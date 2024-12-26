#include <unordered_map>
#include <vector>

#include "treenode.h"
using namespace std;

class Solution {
   public:
    /**
        第三个参数没传引用，导致我输出path为null，o(╥﹏╥)o
    */
    bool dfs_my(TreeNode* curr, TreeNode* target, vector<TreeNode*>& path) {
        if (curr == nullptr) {
            return false;
        }
        // 将当前节点加入path
        path.push_back(curr);
        // 检查当前节点是否是目标节点
        if (curr == target) {
            return true;  // 找到了，不要再对path进行修改了
        }
        // 检查左子树
        bool is_find = dfs_my(curr->left, target, path);
        if (is_find) {
            return true;
        }
        // 检查右子树
        bool is_find2 = dfs_my(curr->right, target, path);
        if (is_find2) {
            return true;
        }
        path.pop_back();
        return false;
    }
    TreeNode* lowestCommonAncestor_my(TreeNode* root, TreeNode* p,
                                      TreeNode* q) {
        // 遍历二叉树找到指定节点，保存路径
        // 分别得到p和q的路径，两个路径中最大索引的节点就是公共祖先节点
        vector<TreeNode*> path, path2;
        dfs_my(root, p, path);
        dfs_my(root, q, path2);
        //    for (TreeNode* ele : path) {
        //         cout << ele->val << " ";
        //     }
        int len = min(path.size(), path2.size()), i = 0;
        while (i < len && path[i] == path2[i]) {
            i++;
        }
        return path[i - 1];
    }
    /**
     * LeetCode Official (1) recursion
     */
    TreeNode* ans;
    bool dfs(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (root == nullptr) {
            return false;
        }
        bool lson = dfs(root->left, p, q);
        bool rson = dfs(root->right, p, q);
        if ((lson && rson) ||
            (root->val == p->val || root->val == q->val) && (lson || rson)) {
            ans = root;
        }
        return lson || rson || root->val == p->val || root->val == q->val;
    }
    TreeNode* lowestCommonAncestor_official1(TreeNode* root, TreeNode* p,
                                             TreeNode* q) {
        dfs(root, p, q);
        return ans;
    }

    /**
     * LeetCode Official (2)
     * 和我的思路类似，只不过他是存储所有节点的父节点，然后从下往上找
     */
    unordered_map<int, TreeNode*> fa;  // 因为树中所有节点值都不一样，所以可以使用int作key
    unordered_map<int, bool> vis;
    void build_fa(TreeNode* root) {
        if (root->left != nullptr) {
            fa[root->left->val] = root;
            build_fa(root->left);
        }
        if (root->right != nullptr) {
            fa[root->right->val] = root;
            build_fa(root->right);
        }
    }
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        fa[root->val] = nullptr;  // 这是很有必要的
        build_fa(root);
        while (p != nullptr) {
            vis[p->val] = true;  // 其实可以是1，但是数字给人的感觉好像还需要转换一下，没有真假值来地直观
            p = fa[p->val];
        }
        while (q != nullptr) {
            if (vis[q->val]) {
                return q;
            }
            q = fa[q->val];
        }
        return nullptr;
    }
};