#include "treenode.h"

class Solution {
   public:
    TreeNode* flipTree(TreeNode* root) {
        if (!root) {
            return nullptr;
        }
        TreeNode* l = flipTree(root->left);
        TreeNode* r = flipTree(root->right);
        root->left = r;
        root->right = l;
        return root;
    }
};