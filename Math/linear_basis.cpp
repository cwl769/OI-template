struct Basis {
  int w, s; veci64 b;
  Basis(int n = 60) : w(n), s(0), b(n) {}
  void insert(i64 x) {
    for (int i = w - 1; i >= 0; --i) {
      if (!((x >> i) & 1)) continue;
      if (b[i] == 0) b[i] = x, ++s;
      x ^= b[i];
    }
  }
  bool check(i64 x) const {
    for (int i = w - 1; i >= 0; --i)
      if (((x >> i) & 1)) x ^= b[i];
    return x == 0;
  }
  int size() const { return s; }
};