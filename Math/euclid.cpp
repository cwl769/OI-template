//设计U和R时注意答案在R处统计
//T为实现了乘法的幺半群
template <typename T> T pow(T a, int b) {
  T res; // 初始应为单位元
  for (; b; b >>= 1) {
    if (b & 1) res = res * a;
    a = a * a;
  }
  return res;
}
template <typename T>
T euclid(int a, int b, int c, int n, T U, T R) {
  if (b >= c)
    return pow(U, b / c) * euclid(a, b % c, c, n, U, R);
  if (a >= c)
    return euclid(a % c, b, c, n, U, pow(U, a / c) * R);
  auto m = ((long long)a * n + b) / c;
  if (!m) return pow(R, n);
  return pow(R, (c - b - 1) / a) * U *
         euclid(c, (c - b - 1) % a, a, m - 1, R, U) *
         pow(R, n - (c * m - b - 1) / a);
}