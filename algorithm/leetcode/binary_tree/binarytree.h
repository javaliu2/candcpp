#ifndef BINARY_TREE
#define BINARY_TREE
#include "treenode.h"
#include <vector>
class BinaryTree {
    private:
        TreeNode *root;
        void pre_order(TreeNode* node) const;
        void in_order(TreeNode* node) const;
        void post_order(TreeNode* node) const;
        void layer_order() const;
    public:
        BinaryTree();
        BinaryTree(std::vector<int>& nums);
        void print(int choice);
        ~BinaryTree();
};
#endif