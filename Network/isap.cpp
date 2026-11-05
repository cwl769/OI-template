template <typename cap_t, typename fee_t = null_t>
cap_t isap(int n, Network<cap_t, fee_t> g, int s, int t) {
veci d, cur, gap;
auto bfs = [&]() -> bool {
  std::queue<int> q;
  d.assign(g.h.size(), INT_MAX);gap.assign(g.h.size()+2, 0);
  d[t] = 0;q.emplace(t);gap[0] = 1;
  while (q.size()) {
    int x = q.front();q.pop();
    for (int i = g.h[x]; i; i = g.edg[i].next) {
      int y = g.edg[i].to;if (g.edg[i^1].cap <= 0)continue;
      if (d[x] + 1 < d[y]) {
        d[y] = d[x] + 1;++gap[d[y]];q.emplace(y);}}}
  return d[s] != INT_MAX;};
auto dfs = [&](auto& self, int x, cap_t lim) -> cap_t {
  if (x == t || lim <= 0) return lim; cap_t flow = 0;
  for (int i = cur[x]; i; cur[x] = i = g.edg[i].next) {
    int y = g.edg[i].to;
    if(g.edg[i].cap<=0)continue; if(d[x]-1!=d[y])continue;
    cap_t tmp=self(self,y,std::min(lim-flow,g.edg[i].cap));
    flow += tmp; g.edg[i].cap -= tmp; g.edg[i^1].cap += tmp;
    if (flow >= lim || d[s] == n) return flow;}
  if (--gap[d[x]] <= 0) {d[s] = n;return flow;}
  ++d[x];++gap[d[x]];cur[x] = g.h[x];return flow;};
cap_t flow = 0; bfs(); cur = g.h;
while(d[s]<n)
  flow+=dfs(dfs,s,std::numeric_limits<cap_t>::max());
return flow;}