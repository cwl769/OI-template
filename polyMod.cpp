int log2(int x) {
    int ans = -1;
    for(;x;x>>=1)++ans;
    return ans;
}

template<i64 MOD>
class Int {
private:
    int v;
public:
    Int():v(){}
    Int(const Int& b) {
        v = b.v;
    }
    Int(Int&& b) {
        v = b.v;
    }
    Int(i64 x) {
        if(x<0) {
            v = (x%MOD)+MOD;
            if(v==MOD)v = 0;
        } else if (x >= MOD) {
            v = x % MOD;
        } else {
            v = x;
        }
    }
    Int(int x) {
        if(x<0) {
            v = (x%MOD)+MOD;
            if(v==MOD)v = 0;
        } else if (x >= MOD) {
            v = x % MOD;
        } else {
            v = x;
        }
    }
    Int& operator = (const Int& b) {
        v = b.v;
        return *this;
    }
    Int& operator = (Int&& b) {
        v = b.v;
        return *this;
    }
    Int operator + (Int b) const {
        Int ans;
        ans.v = v + b.v;
        if(ans.v >= MOD)
            ans.v -= MOD;
        return ans;
    }
    Int operator - (Int b) const {
        Int ans;
        ans.v = v - b.v;
        if(ans.v < 0)
            ans.v += MOD;
        return ans;
    }
    Int operator * (Int b) const {
        return 1ll * v * b.v;
    }
    Int& operator += (Int b) {
        v += b.v;
        if(v >= MOD)
            v -= MOD;
        return *this;
    }
    Int& operator -= (Int b) {
        v -= b.v;
        if(v < 0)
            v += MOD;
        return *this;
    }
    Int& operator *= (Int b) {
        v = (1ll * v * b.v) % MOD;
        return *this;
    }
    Int operator ^ (ui64 b) const {
        Int ans = 1;
        Int base = *this;
        for(;b;b>>=1) {
            if(b&1)
                ans *= base;
            base *= base;
        }
        return ans;
    }
    i64 val() const {return v;}
};

template<i64 MOD>
std::vector<Int<MOD> > NTT(std::vector<Int<MOD> > a, int len, i64 g) {
    len = (1<<(log2(len-1)+1));
    a.resize(len);
    int log2len = log2(len);
    for(int i=1,j=len/2;i<len-1;++i) {
        if(i<j) {
            std::swap(a[i], a[j]);
        }

        int k = len/2;
        while(j >= k) {
            j -= k;
            k>>=1;
        }
        j += k;
    }
    for(int t=0;t<log2len;++t) {
        int n_2 = (1<<t);
        int n = (n_2<<1);
        Int<MOD> wn(Int<MOD>(g)^((MOD-1)/n));
        for(int j=0;j<len;j+=n) {
            Int<MOD> w(1);
            for(int i=j;i<j+n_2;++i) {
                Int<MOD> ta = a[i], tb = w * a[i+n_2];
                a[i] = ta + tb;
                a[i+n_2] = ta - tb;
                w *= wn;
            }
        }
    }
    return a;
}


template<i64 MOD, i64 g>
class PolyMod {
private:
public:
    std::vector<Int<MOD> > v;
    int deg() const {return v.size()-1;}
    PolyMod():v(){}
    PolyMod(std::vector<Int<MOD> >&& vec) {
        v = std::move(vec);
    }
    PolyMod(Int<MOD> x) {v.emplace_back(x);}
    PolyMod(int x) {v.emplace_back(x);}
    PolyMod(i64 x) {v.emplace_back(x);}
    PolyMod(const PolyMod& b) {v = b.v;}
    PolyMod(PolyMod&& b) {v = std::move(b.v);}
    PolyMod& cut(int n) {
        v.resize(n);
        return *this;
    }
    PolyMod operator + (const PolyMod& b) const {
        PolyMod ans;
        int this_deg = deg();
        int b_deg = b.deg();
        ans.v.resize(std::max(this_deg+1, b_deg+1));
        int ans_deg = ans.deg();
        for(int i=0;i<=ans_deg;++i)
            ans.v[i] = ((i<=this_deg)?v[i]:0) + ((i<=b_deg)?b.v[i]:0);
        return ans;
    }
    PolyMod operator - (const PolyMod& b) const {
        PolyMod ans;
        int this_deg = deg();
        int b_deg = b.deg();
        ans.v.resize(std::max(this_deg+1, b_deg+1));
        int ans_deg = ans.deg();
        for(int i=0;i<=ans_deg;++i)
            ans.v[i] = ((i<=this_deg)?v[i]:0) - ((i<=b_deg)?b.v[i]:0);
        return ans;
    }
    PolyMod operator * (const PolyMod& b) const {
        std::vector<Int<MOD> > pa, pb;
        pa = NTT(v, deg()+b.deg()+1, g);
        pb = NTT(b.v, deg()+b.deg()+1, g);
        int n = pa.size();
        for(int i=0;i<n;++i)
            pa[i] *= pb[i];
        pa = NTT(pa, n, g);
        Int<MOD> n_inv = (Int<MOD>(n)^(MOD-2));
        pb[0] = pa[0] * n_inv;
        for(int i=1;i<(int)pb.size();++i) {
            pb[i] = pa[n-i] * n_inv;
        }
        pb.resize(deg()+b.deg()+1);
        return pb;
    }
    PolyMod inv(int n) const {
        if(n == 1) {
            return PolyMod(v[0]^(MOD-2));
        }
        PolyMod f0 = this->inv((n+1)>>1);
        PolyMod tmp = f0 * (PolyMod(2) - (*this * f0).cut(n)).cut(n);
        return tmp.cut(n);
    }
    friend std::ostream& operator << (std::ostream& stream, const PolyMod& poly) {
        for(auto x:poly.v) {
            stream << x.val() << ' ' ;
        }
        return stream;
    }
};