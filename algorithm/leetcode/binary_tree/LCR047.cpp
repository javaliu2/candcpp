#include <iostream>
#include "treenode.h"

// delete会报错，不知道为什么
class Solution {
public:
    bool dfs(TreeNode* node) {
        if (!node) {
            return true;
        }
        bool left_flag = dfs(node->left);
        bool right_flag = dfs(node->right);
        if (left_flag) {
            //delete node->left;
            node->left = nullptr;
        }
        if (right_flag) {
            //delete node->right;
            node->right = nullptr;
        }
        return left_flag && right_flag && node->val == 0;
    }
    TreeNode* pruneTree(TreeNode* root) {
        bool flag = dfs(root);
        if (flag) {
            //delete root;
            return nullptr;
        }
        return root;
    }
};