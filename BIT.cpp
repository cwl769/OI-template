#include <cstdlib>

struct BIT {
    int *c;
    int N;
    BIT(int n):N(n) {
        c = (int*)calloc(n+1, sizeof(int));
    }
    virtual ~BIT() {
        free(c);
    }
    void add(int x, int v) {
        for(;x<=N;x+=(x&(-x)))
            c[x] += v;
    }
    int query(int x) {
        int ans = 0;
        for(;x;x-=(x&(-x)))
            ans += c[x];
        return ans;
    }
};