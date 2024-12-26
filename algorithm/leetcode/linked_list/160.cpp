#include "listnode.h"
#include <unordered_set>
using namespace std;

class Solution {
public:
/**
    * LeetCode Official (2) two pointer, consume 48ms
    */
    ListNode *getIntersectionNode(ListNode *l, ListNode *l2) {
        ListNode *p = l, *q = l2; 
        if (p == nullptr || q == nullptr) {
            return nullptr;
        }
        while (p != q) {
            p = p == nullptr ? l2 : p->next;
            q = q == nullptr ? l : q->next;
        }
        return p;  // 均指向nullptr(这也是相等嘛)，或者相同的节点
    }
    /**
    * LeetCode Official (1) hashset, consume 39ms
    */
    ListNode *getIntersectionNode_hs(ListNode *l, ListNode *l2) {
        ListNode *p = l; 
        unordered_set<ListNode*> visited;
        while (p != nullptr) {
            visited.insert(p);
            p = p->next;
        }
        p = l2;
        while (p != nullptr) {
            if (visited.count(p)) {
                return p;
            }
            p = p->next;
        }
        return nullptr;
    }
    /**
     * my solution, brute force
     */
    ListNode *getIntersectionNode_bf(ListNode *l, ListNode *l2) {
        // O(n^2), consume 652ms
        ListNode *p, *q; 
        for (p = l; p != nullptr; p = p->next) {
            for (q = l2; q != nullptr; q = q->next) {
                if (p == q) {
                    return p;
                }
            }
        }
        return nullptr;
    }
};