struct ODT {
    struct Node {
        int l, r;
        mutable i64 v;
        bool operator < (const Node& b) const {
            return this->l < b.l;
        }
    };
    int L, R;
    std::set<Node> set;
    ODT(int l, int r):L(l),R(r+1) {
        set.insert((Node){
            .l = L,
            .r = R,
            .v = -1
        });
    }
    auto split(int p) {
        auto it = set.upper_bound((Node){p, p, 0});
        --it;
        if(p != it->l) {
            int l = it->l;
            int r = it->r;
            i64 val = it->v;
            set.erase(it);
            set.insert((Node){l, p-1, val});
            return set.insert((Node){p, r, val}).first;
        } else {
            return it;
        }
    }
    void assign(int l, int r, i64 v) {
        //we must split r+1 before spliting l
        auto it_r = split(r + 1);
        auto it_l = split(l);
        set.erase(it_l, it_r);
        set.insert((Node){
            .l = l, 
            .r = r,
            .v = v
        });
    }
};