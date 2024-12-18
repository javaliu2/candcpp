struct Node {
    int val;
    Node *next;
    Node() : val(-1), next(nullptr) {}
    Node(int val) : val(val), next(nullptr) {}
    Node(int val, Node *next) : val(val), next(next) {}
};
class MyLinkedList {
   private:
    Node *head;

   public:
    MyLinkedList() { head = new Node(); }

    int get(int index) {
        Node *p = head;
        if (index < 0) {
            return -1;
        }
        index += 1;
        while (index) {
            p = p->next;
            index--;
            if (!p) {
                return -1;
            }
        }
        return p->val;
    }

    void addAtHead(int val) {
        Node *t = new Node(val);
        t->next = head->next;
        head->next = t;
    }

    void addAtTail(int val) {
        Node *p = head->next, *prev = head;
        while (p) {
            prev = p;
            p = p->next;
        }
        Node *t = new Node(val);
        prev->next = t;
    }

    void addAtIndex(int index, int val) {
        if (index < 0) {
            return;
        }
        Node *prev = head, *p = head->next;
        while (index) {
            prev = p;
            if (!p && index) {
                return;
            }
            index--;
            p = p->next;
        }
        Node *t = new Node(val);
        t->next = prev->next;
        prev->next = t;
    }

    void deleteAtIndex(int index) {
        if (index < 0) {
            return;
        }
        Node *p = head->next, *prev = head;
        while (index) {
            prev = p;
            p = p->next;
            index--;
            if (!p) {
                return;
            }
        }
        if (!p) {
            return;
        }
        Node *t = p->next;
        prev->next = t;
        delete p;
    }
};

/**
 * Your MyLinkedList object will be instantiated and called as such:
 * MyLinkedList* obj = new MyLinkedList();
 * int param_1 = obj->get(index);
 * obj->addAtHead(val);
 * obj->addAtTail(val);
 * obj->addAtIndex(index,val);
 * obj->deleteAtIndex(index);
 */