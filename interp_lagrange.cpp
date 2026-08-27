// known f(0 -> k), should init jcinv[0 -> k]
i64 lagrange(const veci64& f, int k, i64 xx) {
  if (xx <= k) return f[xx];
  veci64 pre(k + 1), suf(k + 1);
  pre[0] = suf[k] = 1;
  for (int i = 1; i <= k; ++i)
    pre[i] = (xx - (i - 1)) % MOD * pre[i - 1] % MOD;
  for (int i = k - 1; i >= 0; --i)
    suf[i] = (xx - (i + 1)) % MOD * suf[i + 1] % MOD;
  i64 ans = 0;
  for (int i = 0; i <= k; ++i) {
    i64 tmp = f[i] * pre[i] % MOD * suf[i] % MOD;
    tmp = tmp * jcinv[k - i] % MOD * jcinv[i] % MOD;
    if ((k ^ i) & 1) ans = (ans + MOD - tmp) % MOD;
    else ans = (ans + tmp) % MOD;
  }
  return ans;
}