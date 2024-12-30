#include "../linked_list/listnode.h"
#include "treenode.h"
#include <stack>
#include <unordered_map>

using namespace std;

/**
 * LeetCode Official
 */
class Solution2 {
    bool dfs(TreeNode* rt, ListNode* head) {
        // 链表已经全部匹配完，匹配成功
        if (head == NULL) return true;
        // 二叉树访问到了空节点，匹配失败
        if (rt == NULL) return false;
        // 当前匹配的二叉树上节点的值与链表节点的值不相等，匹配失败
        if (rt->val != head->val) return false;
        return dfs(rt->left, head->next) || dfs(rt->right, head->next);
    }
public:
    bool isSubPath(ListNode* head, TreeNode* root) {
        if (root == NULL) return false;
        return dfs(root, head) || isSubPath(head, root->left) || isSubPath(head, root->right);
    }
};

class Solution {
public:
    /**
    * 61/68个通过的测试用例, 针对head=[4, 2], root=[4,4,4,1,null,null,null,2]
    * 输出true, 预期false，因为这个代码没有考虑连续。只是说tree存在一条路径，
    * 该路径上出现了head的所有元素，中间有不是head中的元素，也可以，这是不对的
    */
    bool dfs(TreeNode* treenode, ListNode* listnode) {
        if (listnode==nullptr) {
            return true;
        } else if (treenode == nullptr && listnode != nullptr) {
            return false;
        }
        if (treenode->val == listnode->val) {
            return dfs(treenode->left, listnode->next) || dfs(treenode->right, listnode->next);
        } 
        return dfs(treenode->left, listnode) || dfs(treenode->right, listnode);
    }

    bool isSubPath(ListNode* head, TreeNode* root) {
        // 定义子问题：dfs(treenode, listnode)
        // 如果treenode.val == listnode.val，则dfs(treenode.left, listnode.next)||dfs(treenode.right, listnode.next)
        // return dfs(root, head);
        return func(head, root);
    }
    /**
    * 思路二，建立fa，保存每一个节点的父节点，遍历fa，找到等于head链表最后一个节点的treenode，从该节点开始
    * 循着父节点往上，依次判断是否等于链表节点的值
    */
    bool func(ListNode* head, TreeNode* root) {
        unordered_map<TreeNode*, TreeNode*> fa;
        dfs2(root, fa);
        fa[root] = nullptr;
        stack<ListNode*> s, st;
        ListNode* p = head;
        while (p != nullptr) {
            s.push(p);
            p = p->next;
        }
        st = s;
        TreeNode* tn;
        /**
         *  如果auto&: 报错 initial value of reference to non-const must be an lvalue
         * 原因是因为 std::pair<TreeNode *const, TreeNode *> 中的key是const，不能被修改
         * 因此，必须使用const关键字修饰auto&，即声明'我不会修改key'
         */
        for (auto iter = fa.cbegin(); iter != fa.cend(); ++iter) {
            tn = iter->first;
            while (tn != nullptr && tn->val == st.top()->val) {
                st.pop();
                tn = fa[tn];
                if (st.empty()) {
                    return true;
                }
            }
            st = s;
        }
        return false;
    }
    void dfs2(TreeNode* node, unordered_map<TreeNode*, TreeNode*>& fa) {
        if (node->left != nullptr) {
            fa[node->left] = node;
            dfs2(node->left, fa);
        }
        if (node->right != nullptr) {
            fa[node->right] = node;
            dfs2(node->right, fa);
        }
    }
};