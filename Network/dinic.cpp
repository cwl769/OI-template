template <typename cap_t, typename fee_t = null_t>
cap_t dinic(Network<cap_t, fee_t> g, int s, int t) {
  veci d, cur;
  auto bfs = [&]() -> bool {
    std::queue<int> q;d.assign(g.h.size(), INT_MAX);
    d[s] = 0;q.emplace(s);
    while (q.size()) { int x = q.front();q.pop();
      for (int i = g.h[x]; i; i = g.edg[i].next) {
        if(g.edg[i].cap <= 0)continue; int y = g.edg[i].to;
        if(d[x] + 1 < d[y]){d[y] = d[x] + 1;q.emplace(y);}}}
    return d[t] != INT_MAX;};
  auto dfs = [&](auto& self, int x, cap_t lim) -> cap_t {
    if (x == t || lim <= 0) return lim;
    cap_t flow = 0;
    for (int i = cur[x]; i; cur[x] = i = g.edg[i].next) {
      int y = g.edg[i].to;
      if (d[x] + 1 != d[y] || g.edg[i].cap <= 0) continue;
      auto tmp=self(self,y,std::min(lim-flow,g.edg[i].cap));
      flow += tmp;g.edg[i].cap -= tmp;g.edg[i^1].cap += tmp;
      if (flow >= lim) return flow;}
    return flow;};
  cap_t flow = 0;
  while (bfs()) { cur = g.h;
    flow += dfs(dfs, s, std::numeric_limits<cap_t>::max());}
  return flow;
}