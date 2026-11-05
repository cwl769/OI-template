struct null_t {auto operator-() const {return *this;}};
template <typename cap_t, typename fee_t = null_t>
struct Network {
  struct Edge {int next, to;cap_t cap;
    [[no_unique_address]] fee_t fee;};
  veci h;std::vector<Edge> edg;Network(int n):h(n),edg(2){}
  void add_oneway(int x, int y, cap_t c, fee_t w) {
    edg.push_back({h[x], y, c, w});h[x]=(int)edg.size()-1;}
  void add(int x, int y, cap_t c, fee_t w = null_t{}) {
    add_oneway(x, y, c, w);add_oneway(y, x, 0, -w);}
};