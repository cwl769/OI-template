template<typename Data,typename Act,typename coord=int>
  requires SegtreeRC_concept<Data, Act>
struct SegtreeRC {
struct Node {
Data data;Act tag;int son[2];Node():data(),tag(),son{0,0}{}
};
inline static std::vector<Node> pl{Node()};
static void clear() { pl.assign(1, Node()); }
static int newNode()
  {pl.emplace_back();return (int)pl.size() - 1;}
int root; coord L, R;
SegtreeRC(coord l, coord r) {root = 0;L = l;R = r;}
void pushup(int p) { pl[p].data =
    merge(pl[pl[p].son[0]].data, pl[pl[p].son[1]].data);
}
void pushdown(int p, coord l, coord r) {
  if(pl[p].son[0] == 0) pl[p].son[0] = newNode();
  if(pl[p].son[1] == 0) pl[p].son[1] = newNode();
  coord mid = l + r >> 1;
  pl[pl[p].son[0]].data.act(pl[p].tag, l, mid);
  pl[pl[p].son[1]].data.act(pl[p].tag, mid + 1, r);
  pl[pl[p].son[0]].tag.compose(pl[p].tag);
  pl[pl[p].son[1]].tag.compose(pl[p].tag);
  pl[p].tag = Act();
}
int chg(int p, coord l, coord r, coord pos, Data val) {
  coord mid = l + r >> 1; if (!p) p = newNode();
  if(l == r) {pl[p].data = val;return p;}
  if(pos<=mid)pl[p].son[0]=chg(pl[p].son[0],l,mid,pos,val);
  else pl[p].son[1]=chg(pl[p].son[1], mid+1, r, pos, val);
  pushup(p);return p;
}void chg(coord pos, Data val){root=chg(root,L,R,pos,val);}
int act(int p,coord l,coord r,coord ql,coord qr,Act dd){
  if(!p) p = newNode();coord mid = l + r >> 1;
  if(ql <= l && r <= qr) { pl[p].data.act(dd, l, r);
    pl[p].tag.compose(dd);return p;} pushdown(p, l, r);
  if(ql <= mid) pl[p].son[0] =
      act(pl[p].son[0], l, mid, ql, qr, dd);
  if(qr > mid) pl[p].son[1] =
      act(pl[p].son[1], mid + 1, r, ql, qr, dd);
  pushup(p);return p;
}void act(coord l,coord r,Act d){root=act(root,L,R,l,r,d);}
Data query(int p, coord l, coord r, coord ql, coord qr) {
  coord mid = l + r >> 1;if(!p) return pl[0].data;
  if(ql<=l&&r<=qr){return pl[p].data;}pushdown(p, l, r);
  if(qr <= mid)return query(pl[p].son[0], l, mid, ql,qr);
  if(ql > mid)return query(pl[p].son[1], mid+1, r, ql,qr);
  return merge(query(pl[p].son[0], l, mid, ql, qr),
              query(pl[p].son[1], mid + 1, r, ql, qr));
}Data query(coord l, coord r){return query(root,L,R,l,r);}
};