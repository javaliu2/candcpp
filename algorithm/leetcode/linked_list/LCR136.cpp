#include "listnode.h"
class Solution {
public:
    ListNode* deleteNode(ListNode* head, int val) {
        ListNode* prev = head, *p = head->next;
        if (val == head->val) {
            return head->next;
        }
        while (p) {
            if (p->val == val) {
                prev->next = p->next;
                p->next = nullptr;
                break;
            }
            prev = p;
            p = p->next;
        }
        return head;
    }
};