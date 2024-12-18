#include "binarytree.h"

#include <iostream>
#include <queue>
using namespace std;

void BinaryTree::pre_order(TreeNode* node) const {
    if (!node) {
        return;
    }
    cout << node->val << " ";
    pre_order(node->left);
    pre_order(node->right);
}

void BinaryTree::in_order(TreeNode* node) const {
    if (!node) {
        return;
    }
    in_order(node->left);
    cout << node->val << " ";
    in_order(node->right);
}

void BinaryTree::post_order(TreeNode* node) const {
    if (!node) {
        return;
    }
    post_order(node->left);
    post_order(node->right);
    cout << node->val << " ";
}

void BinaryTree::layer_order() const {
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; i++) {
            TreeNode* t = q.front();
            q.pop();
            cout << t->val << " ";
            if (t->left) {
                q.push(t->left);
            }
            if (t->right) {
                q.push(t->right);
            }
        }
        cout << endl;
    }
}

BinaryTree::BinaryTree() : root(nullptr) {}

// 根据bfs构建二叉树
BinaryTree::BinaryTree(std::vector<int>& nums) {
    queue<TreeNode*> q;
    root = new TreeNode(nums[0]);
    q.push(root);
    for (int i = 1; i < nums.size(); i+=2) {
        TreeNode *first = q.front();
        q.pop();
        if (nums[i] != -1) {
            first->left = new TreeNode(nums[i]);
            q.push(first->left);
        }
        if (i + 1 < nums.size() && nums[i + 1] != -1) {
            first->right = new TreeNode(nums[i + 1]);
            q.push(first->right);
        }
    }
}

// 树的遍历
// choice 0: 前序遍历; 1: 中序遍历; 2: 后序遍历; 3: 层次遍历
void BinaryTree::print(int choice) {
    switch (choice) {
        case 0:
            pre_order(root);
            break;
        case 1:
            in_order(root);
            break;
        case 2:
            post_order(root);
            break;
        case 3:
            layer_order();
            break;
        default:
            cout << "wrong parameter, candidates are 0,1,2,3, " << 
            "respectively corresponding pre_order, in_order, post_order, layer_order" << endl;
            break;
    }
}

BinaryTree::~BinaryTree() { delete root; }
