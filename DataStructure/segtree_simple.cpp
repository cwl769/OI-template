template <typename Data, typename coord = int>
  requires Mergeable<Data>
struct Segtree {
struct Node{Data data;int son[2];Node():data(),son{0,0}{}};
inline static std::vector<Node> pl{Node()};
static void clear() {pl.assign(1, Node());}
static int newNode()
  {pl.emplace_back();return (int)pl.size() - 1;}
int root; coord L, R;
Segtree(coord l, coord r) {root = 0;L = l;R = r;}
void pushup(int p) { pl[p].data =
    merge(pl[pl[p].son[0]].data, pl[pl[p].son[1]].data);
}
int chg(int p, coord l, coord r, coord pos, Data val) {
  coord mid = l + r >> 1; if (!p) p = newNode();
  if(l == r) {pl[p].data = val;return p;}
  if(pos<=mid)pl[p].son[0]=chg(pl[p].son[0],l,mid,pos,val);
  else pl[p].son[1]=chg(pl[p].son[1], mid+1, r, pos, val);
  pushup(p);return p;
}
void chg(coord pos, Data val){root=chg(root,L,R,pos,val);}
// When data is "Addable", the following 2 func exist.
template <typename Delta> requires Addable<Data, Delta>
int add(int p, coord l, coord r, coord pos, Delta dd) {
  coord mid = l + r >> 1; if (!p) p = newNode();
  if(l == r) {pl[p].data += dd;return p;}
  if(pos<=mid)pl[p].son[0]=add(pl[p].son[0],l,mid,pos,dd);
  else pl[p].son[1]=add(pl[p].son[1], mid+1, r, pos, dd);
  pushup(p);return p;
}
template <typename Delta> requires Addable<Data, Delta>
void add(coord pos, Delta dd){root=add(root,L,R,pos,dd);}
Data query(int p, coord l, coord r, coord ql, coord qr) {
  coord mid = l + r >> 1; if (!p) return pl[0].data;
  if(ql <= l && r <= qr) return pl[p].data;
  if(qr <= mid)return query(pl[p].son[0], l, mid, ql,qr);
  if(ql > mid)return query(pl[p].son[1], mid+1, r, ql,qr);
  return merge(query(pl[p].son[0], l, mid, ql, qr),
                query(pl[p].son[1], mid + 1, r, ql, qr));
}
Data query(coord l, coord r) {return query(root,L,R,l,r);}
};