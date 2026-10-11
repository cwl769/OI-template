template <typename Data, typename coord = int>
  requires Mergeable<Data>
struct Segtree {
  struct Node {
    Data data;
    int son[2];
    Node() : data(), son{0, 0} {}
  };
  inline static std::vector<Node> pool{Node()};
  static void clear() { pool.assign(1, Node()); }
  static int newNode() {
    pool.emplace_back();
    return (int)pool.size() - 1;
  }
  int root;
  coord L, R;
  Segtree(coord l, coord r) {
    root = 0;L = l;R = r;
  }
  void pushup(int ptr) {
    pool[ptr].data = merge(pool[pool[ptr].son[0]].data,
                           pool[pool[ptr].son[1]].data);
  }
  int change(int ptr, coord l, coord r, coord pos,
             Data val) {
    if (!ptr) ptr = newNode();
    if (l == r) {
      pool[ptr].data = val;
      return ptr;
    }
    coord mid = l + r >> 1;
    if (pos <= mid)
      pool[ptr].son[0] =
        change(pool[ptr].son[0], l, mid, pos, val);
    else
      pool[ptr].son[1] =
        change(pool[ptr].son[1], mid + 1, r, pos, val);
    pushup(ptr);
    return ptr;
  }
  void change(coord pos, Data val) {
    root = change(root, L, R, pos, val);
  }
  // When data is "Addable", the following 2 func exist.
  template <typename Delta>
    requires Addable<Data, Delta>
  int add(int ptr, coord l, coord r, coord pos, Delta dd) {
    if (!ptr) ptr = newNode();
    if (l == r) {
      pool[ptr].data.add(dd);
      return ptr;
    }
    coord mid = l + r >> 1;
    if (pos <= mid)
      pool[ptr].son[0] =
        add(pool[ptr].son[0], l, mid, pos, dd);
    else
      pool[ptr].son[1] =
        add(pool[ptr].son[1], mid + 1, r, pos, dd);
    pushup(ptr);
    return ptr;
  }
  template <typename Delta>
    requires Addable<Data, Delta>
  void add(coord pos, Delta dd) {
    root = add(root, L, R, pos, dd);
  }
  Data query(int ptr, coord l, coord r, coord ql,
             coord qr) const {
    if (!ptr) return pool[0].data;
    if (ql <= l && r <= qr) return pool[ptr].data;
    coord mid = l + r >> 1;
    if (qr <= mid)
      return query(pool[ptr].son[0], l, mid, ql, qr);
    if (ql > mid)
      return query(pool[ptr].son[1], mid + 1, r, ql, qr);
    return merge(
      query(pool[ptr].son[0], l, mid, ql, qr),
      query(pool[ptr].son[1], mid + 1, r, ql, qr));
  }
  Data query(coord l, coord r) {
    return query(root, L, R, l, r);
  }
};