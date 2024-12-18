#include "listnode.h"
#include <vector>
using namespace std;


class Solution {
public:
    /**
     * reference of pointer, wuwuwu, it's so familiar
     */
    void my_merge(ListNode* & res, ListNode* & target) {
        // search previous Node of target[i]
        ListNode *prev = new ListNode(0, res), *head = prev;
        ListNode *p = res, *q = target, *t;
        while (q) {
            t = q;
            while (p && p->val <= t->val) {
                prev = p;
                p = p->next;
            }
            q = q->next;
            prev->next = t;
            t->next = p;
            prev = t;
        }
        res = head->next;
        delete head;
    }
    /**
     * 手到擒来
     */
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        int n = lists.size();
        if (n == 0) {
            return nullptr;
        }
        ListNode* res = lists[0];
        for (int i = 1; i < n; i++) {
            my_merge(res, lists[i]);
        }
        return res;
    }
};