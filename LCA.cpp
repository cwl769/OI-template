#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>

int log2_floor(int x) {
    int ans = 0;
    while(x) {
        ++ans;
        x >>= 1;
    }
    return ans - 1;
}

int log2_ceil(int x) {
    int i = 0;
    for(;i<=30;++i) {
        if((1<<i) >= x)
            break;
    }
    return i;
}

class LCA final{
private:
    const int N, layer;
    int **fa;
public:
    int *dep;
    LCA(int _n):N(_n), layer(log2_ceil(_n) + 1) {
        fa = (int**)malloc(layer * sizeof(int*));
        for(int i=0;i<layer;++i) {
            fa[i] = (int*)malloc(N*sizeof(int));
        }
        dep = (int*)malloc(N*sizeof(int));
    }
    ~LCA() {
        free(dep);
        for(int i=0;i<layer;++i) {
            free(fa[i]);
        }
        free(fa);
    }
    void setFather(int x, int father) {
        fa[0][x] = father;
    }
    void setFather(const int *father) {
        memcpy(fa[0], father, N*sizeof(int));
    }
    void setDepth(int x, int depth) {
        dep[x] = depth;
    }
    void setDepth(const int *depth) {
        memcpy(dep, depth, N*sizeof(int));
    }
    void calDepth() {
        memset(dep, 0x00, N*sizeof(int));
        for(int t=0;t<N;++t) {
            int x = t;
            for(int i=layer-1;i>=0;--i) {
                if(fa[0][fa[i][x]] != fa[i][x]) {
                    dep[t] += (1<<i);
                    x = fa[i][x];
                }
            }
            if(fa[0][x] == x) {
                dep[t] = 0;
            } else {
                ++dep[t];
            }
        }
    }
    void finishInput() {
        for(int i=1;i<layer;++i) {
            for(int x=0;x<N;++x) {
                fa[i][x] = fa[i-1][fa[i-1][x]];
            }
        }
    }
    int query(int x, int y) const {
        if(dep[x] > dep[y]) {
            std::swap(x, y);
        }
        if(dep[x] != dep[y]) {
            for(int i=layer-1;i>=0;--i)
                if(dep[fa[i][y]] >= dep[x])
                    y = fa[i][y];
        }
        if(x == y)return x;
        for(int i=layer-1;i>=0;--i) {
            if(fa[i][x] != fa[i][y]) {
                x = fa[i][x];
                y = fa[i][y];
            }
        }
        return fa[0][x];
    }
    int operator () (int x, int y) const {
        return query(x, y);
    }
};