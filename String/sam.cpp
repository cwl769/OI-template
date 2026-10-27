template <typename ch_t> struct SAM {
struct Node {
  int len, id, link;std::map<ch_t, int> next;
  Node(int _id) : len(), id(_id), link(-1) {}
};std::vector<Node> pl;
int newNode(){int s=pl.size();pl.emplace_back(s);return s;}
int root, last; SAM(){last=root=newNode();}
int append(ch_t c) {
  int cur=newNode(),p=last;pl[cur].len=pl[last].len+1;
  for (; p != -1; p = pl[p].link) {
    if (pl[p].next.find(c) != pl[p].next.end())break;
    else pl[p].next[c] = cur;
  }
  if(p == -1){pl[cur].link = root;return last = cur;}
  int q = pl[p].next[c];
  if (pl[p].len + 1 == pl[q].len) {pl[cur].link = q;}
  else { int cq = newNode(); pl[cq].len = pl[p].len + 1;
    pl[cq].link = pl[q].link; pl[cq].next = pl[q].next;
    pl[q].link = cq;pl[cur].link = cq;
    for (;p!=-1 && pl[p].next.find(c)!=pl[p].next.end()
    && pl[p].next[c]==q;p=pl[p].link){pl[p].next[c]=cq;}
  }
  return last = cur;
}};