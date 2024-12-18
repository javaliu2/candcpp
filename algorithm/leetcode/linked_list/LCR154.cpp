#include <map>
using namespace std;

// Definition for a Node.
class Node {
   public:
    int val;
    Node* next;
    Node* random;

    Node(int _val) {
        val = _val;
        next = nullptr;
        random = nullptr;
    }
};

class Solution {
   public:
    Node* copyRandomList(Node* head) {
        // 实现该Node对象的拷贝构造
        // 1、拷贝值和next, 建立head链表节点和我new的节点之间的映射
        map<Node*, Node*> mapping;
        Node *p = head, *q;
        Node res(0), *prev = &res;
        while (p) {
            q = new Node(p->val);
            mapping[p] = q;
            p = p->next;
            prev->next = q;
            prev = q;
        }
        // 2、fill random field
        p = head, q = res.next;
        while (p) {
            q->random = mapping[p->random];
            q = q->next;
            p = p->next;
        }
        return res.next;
    }
};