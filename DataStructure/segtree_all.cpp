#include <cstdio>
#include <vector>
#include <concepts>
typedef long long i64;

template<typename Data>
concept Mergeable = requires(Data a, Data b) {
    {merge(a, b)} -> std::convertible_to<Data>;
};

template<typename Data, typename Delta>
concept Addable = requires(Data a, Delta d) {a += d;};

template<typename Data, typename Action>
concept RangeChangeable = requires(Data &a, Action b) {
    a.act(b, 1ll, 1ll);
};

template<typename Action>
concept Composable = requires(Action a, Action b) {
    a.compose(b);
};

template<typename Data, typename Action>
concept SegtreeRC_concept = 
    Mergeable<Data> && 
    RangeChangeable<Data, Action> &&
    Composable<Action>;

template<typename Data, typename coord = int>
    requires Mergeable<Data>
struct SegtreeSimple {
    struct Node {
        Data data;
        int son[2];
        Node():data(), son{0, 0} {}
    };
    inline static std::vector<Node> pl{Node()};
    static void clear() { pl.assign(1, Node()); }
    static int newNode() {
        pl.emplace_back();
        return (int)pl.size() - 1;
    }

    int root;
    coord L, R;
    SegtreeSimple(coord l, coord r) {
        root = 0;
        L = l;R = r;
    }

    void pushup(int p) {
        pl[p].data = merge(pl[pl[p].son[0]].data, pl[pl[p].son[1]].data);
    }
    int change(int p, coord l, coord r, coord pos, Data val) {
        if(!p) p = newNode();
        if(l == r) {
            pl[p].data = val;
            return p;
        }
        coord mid = l + r >> 1;
        if(pos <= mid)
            pl[p].son[0] = change(pl[p].son[0], l, mid, pos, val);
        else
            pl[p].son[1] = change(pl[p].son[1], mid + 1, r, pos, val);
        pushup(p);
        return p;
    }
    void change(coord pos, Data val) {root = change(root, L, R, pos, val);}
    //When data is "Addable", the following two function exist.
    template<typename Delta> requires Addable<Data, Delta>
    int add(int p, coord l, coord r, coord pos, Delta dd) {
        if(!p) p = newNode();
        if(l == r) {
            pl[p].data += dd;
            return p;
        }
        coord mid = l + r >> 1;
        if(pos <= mid)
            pl[p].son[0] = add(pl[p].son[0], l, mid, pos, dd);
        else 
            pl[p].son[1] = add(pl[p].son[1], mid + 1, r, pos, dd);
        pushup(p);
        return p;
    }
    template<typename Delta> requires Addable<Data, Delta>
    void add(coord pos, Delta dd) {root = add(root, L, R, pos, dd);}
    Data query(int p, coord l, coord r, coord ql, coord qr) const {
        if(!p) return pl[0].data;
        if(ql <= l && r <= qr)
            return pl[p].data;
        coord mid = l + r >> 1;
        if(qr <= mid) return query(pl[p].son[0], l, mid, ql, qr);
        if(ql > mid) return query(pl[p].son[1], mid + 1, r, ql, qr);
        return merge(query(pl[p].son[0], l, mid, ql, qr),
                query(pl[p].son[1], mid + 1, r, ql, qr));
    }
    Data query(coord l, coord r){return query(root, L, R, l, r);}
};

template<typename Data, typename Action, typename coord = int>
    requires SegtreeRC_concept<Data, Action>
struct SegtreeRC {
    struct Node {
        Data data;
        Action tag;
        int son[2];
        Node():data(), tag(), son{0, 0} {}
    };
    inline static std::vector<Node> pl{Node()};
    static void clear() { pl.assign(1, Node()); }
    static int newNode() {
        pl.emplace_back();
        return (int)pl.size() - 1;
    }

    int root;
    coord L, R;
    SegtreeRC(coord l, coord r) {
        root = 0;
        L = l; R = r;
    }
    void pushup(int p) {
        pl[p].data = merge(pl[pl[p].son[0]].data, pl[pl[p].son[1]].data);
    }
    void pushdown(int p, coord l, coord r) {
        if(pl[p].son[0] == 0)pl[p].son[0] = newNode();
        if(pl[p].son[1] == 0)pl[p].son[1] = newNode();
        coord mid = l + r >> 1;
        pl[pl[p].son[0]].data.act(pl[p].tag, l, mid);
        pl[pl[p].son[1]].data.act(pl[p].tag, mid + 1, r);
        pl[pl[p].son[0]].tag.compose(pl[p].tag);
        pl[pl[p].son[1]].tag.compose(pl[p].tag);
        pl[p].tag = Action();
    }

    int change(int p, coord l, coord r, coord ql, coord qr, Action dd) {
        if(!p) p = newNode();
        if(ql <= l && r <= qr) {
            pl[p].data.act(dd, l, r);
            pl[p].tag.compose(dd);
            return p;
        }
        coord mid = l + r >> 1;
        pushdown(p, l, r);
        if(ql <= mid)
            pl[p].son[0] = change(pl[p].son[0], l, mid, ql, qr, dd);
        if(qr > mid)
            pl[p].son[1] = change(pl[p].son[1], mid + 1, r, ql, qr, dd);
        pushup(p);
        return p;
    }
    void change(coord l, coord r, Action dd) {root = change(root, L, R, l, r, dd);}

    Data query(int p, coord l, coord r, coord ql, coord qr) {
        if(!p) return pl[0].data;
        if(ql <= l && r <= qr)
            return pl[p].data;
        coord mid = l + r >> 1;
        pushdown(p, l, r);
        if(qr <= mid) return query(pl[p].son[0], l, mid, ql, qr);
        if(ql > mid) return query(pl[p].son[1], mid + 1, r, ql, qr);
        return merge(query(pl[p].son[0], l, mid, ql, qr),
                query(pl[p].son[1], mid + 1, r, ql, qr));
    }
    Data query(coord l, coord r){return query(root, L, R, l, r);}
};

struct RangeAdd {
    i64 tag;
    RangeAdd():tag(){}
    RangeAdd(i64 x):tag(x){}
    void compose(RangeAdd b) {
        tag += b.tag;
    }
};

struct RangeSum {
    i64 sum;
    RangeSum():sum(){}
    RangeSum(i64 x):sum(x){}
    RangeSum& operator += (i64 dd) {
        sum += dd;
        return *this;
    }
    void act(RangeAdd dd, int l, int r) {
        sum += dd.tag * (r - l + 1ll);
    }
};
RangeSum merge(RangeSum a, RangeSum b) {
    return a.sum + b.sum;
}


int main() {
    int n, m;scanf("%d%d", &n, &m);
    SegtreeRC<RangeSum, RangeAdd> tree(1, n);
    // printf("%d %d\n", n, m);
    for (int i = 1; i <= n; ++i) {
        i64 x;scanf("%lld", &x);
        tree.change(i, i, x);
    }
    for (int rnd = 1; rnd <= m; ++rnd) {
        int tp, l, r;scanf("%d%d%d", &tp, &l, &r);
        if(tp == 1) {
            i64 delta;scanf("%lld", &delta);
            tree.change(l, r, delta);
        } else {
            printf("%lld\n", tree.query(l, r).sum);
        }
    }


    return 0;
}