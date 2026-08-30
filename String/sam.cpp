int tot = -1;
struct SAM {
  struct Node {
    int id, len;
    Node* link;
    std::map<char, Node*> next;
    Node() {
      ++tot;
      id = tot;
      len = 0;
      link = NULL;
    }
  };

  std::vector<Node*> all_nodes;

  Node *root, *last;
  SAM() {
    root = new Node;
    all_nodes.emplace_back(root);
    last = root;
  }
  Node* append(char c) {
    Node* cur = new Node;
    all_nodes.emplace_back(cur);
    cur->len = last->len + 1;
    Node* p = last;
    for (; p != NULL; p = p->link) {
      if (p->next.find(c) != p->next.end()) {
        break;
      } else {
        p->next[c] = cur;
      }
    }
    if (p == NULL) {
      cur->link = root;
      last = cur;
      return cur;
    }
    Node* q = p->next[c];
    if (p->len + 1 == q->len) {
      cur->link = q;
    } else {
      Node* cq = new Node;
      all_nodes.emplace_back(cq);
      cq->len = p->len + 1;
      cq->link = q->link;
      cq->next = q->next;

      q->link = cq;
      cur->link = cq;

      for (;
           p != NULL && p->next.find(c) != p->next.end() &&
           p->next[c] == q;
           p = p->link) {
        p->next[c] = cq;
      }
    }
    last = cur;
    return cur;
  }
};