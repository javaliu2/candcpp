#include "linkedlist.h"
#include <vector>
using namespace std;

class Solution {
public:
    bool isPalindrome(ListNode* head) {
        vector<int> l;
        ListNode* p = head;
        while (p) {
            l.push_back(p->val);
            p = p->next;
        }
        int i = 0, j = l.size() - 1;
        while (i < j) {
            if (l[i] != l[j]) {
                return false;
            }
            i++;
            j--;
        }
        return true;
    }
};