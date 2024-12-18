
// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

/* failure version */
ListNode *getIntersectionNode_my(ListNode *p1, ListNode *p2) {
    int round = 0;
    while (p1 != nullptr && p2 != nullptr) {
        if (p1 == p2) {
            return p1;
        }
        if (round & 1) {
            p1 = p1->next;
        } else {
            p2 = p2->next;
        }
        round++;
    }
    return nullptr;
}
/* leetcode official */
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    if (headA == nullptr || headB == nullptr) {
        return nullptr;
    }
    ListNode *pA = headA, *pB = headB;
    while (pA != pB) {
        pA = pA == nullptr ? headB : pA->next;
        pB = pB == nullptr ? headA : pB->next;
    }
    return pA;
}