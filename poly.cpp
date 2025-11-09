#include <cstdio>
#include <cmath>
#include <cstring>
#include <vector>
#include <complex>
#include <iostream>

typedef long long int64;
typedef unsigned long long uint64;
const double pi = acos(-1);

int log2(int x) {
    int ans = -1;
    for(;x;x>>=1)++ans;
    return ans;
}

template<int64 M>
class Int {
private:
    int64 v;
public:
    Int():v(){}
    Int(const Int& b) {
        v = b.v;
    }
    Int(Int&& b) {
        v = b.v;
    }
    Int(int64 x) {
        if(x<0) {
            v = (x%M)+M;
            if(v==M)v = 0;
        } else if (x >= M) {
            v = x % M;
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
        return v + b.v;
    }
    Int operator - (Int b) const {
        return v - b.v;
    }
    Int operator * (Int b) const {
        return v * b.v;
    }
    Int& operator += (Int b) {
        v += b.v;
        if(v >= M)
            v -= M;
        return *this;
    }
    Int& operator -= (Int b) {
        v -= b.v;
        if(v < 0)
            v += M;
        return *this;
    }
    Int& operator *= (Int b) {
        v = (v * b.v) % M;
        return *this;
    }
    Int operator ^ (uint64 b) const {
        Int ans = 1;
        Int base = *this;
        for(;b;b>>=1) {
            if(b&1)
                ans *= base;
            base *= base;
        }
        return ans;
    }
    int64 val() const {return v;}
};

std::vector<std::complex<double> > FFT(std::vector<std::complex<double> > a, int len) {
    // std::vector<std::complex<double> > ans;
    len = (1<<(log2(len-1)+1));
    a.resize(len);
    // ans.resize(len);
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
        std::complex<double> wn(cos(pi/n_2), sin(pi/n_2));
        // std::complex<double> w(1, 0);
        for(int j=0;j<len;j+=n) {
            std::complex<double> w(1, 0);
            for(int i=j;i<j+n_2;++i) {
                std::complex<double> ta = a[i], tb = w * a[i+n_2];
                a[i] = ta + tb;
                a[i+n_2] = ta - tb;
                w *= wn;
            }
        }
    }
    return a;
}

template<int64 M>
std::vector<Int<M> > NTT(std::vector<Int<M> > a, int len, int64 g) {
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
        Int<M> wn(Int<M>(g)^((M-1)/n));
        // Int<M> w(1, 0);
        for(int j=0;j<len;j+=n) {
            Int<M> w(1);
            for(int i=j;i<j+n_2;++i) {
                Int<M> ta = a[i], tb = w * a[i+n_2];
                a[i] = ta + tb;
                a[i+n_2] = ta - tb;
                w *= wn;
            }
        }
    }
    return a;
}

class Poly {
private:
public:
    std::vector<std::complex<double> > v;
    int deg() const {return v.size()-1;}
    Poly():v(){}
    Poly(std::vector<std::complex<double> >&& vec) {
        v = std::move(vec);
    }
    Poly operator + (const Poly& b) const {
        Poly ans;
        ans.v.resize(std::max(deg()+1, b.deg()+1));
        for(int i=0;i<=ans.deg();++i)
            ans.v[i] = v[i] + b.v[i];
        return ans;
    }
    Poly operator - (const Poly& b) const {
        Poly ans;
        ans.v.resize(std::max(deg()+1, b.deg()+1));
        for(int i=0;i<=ans.deg();++i)
            ans.v[i] = v[i] - b.v[i];
        return ans;
    }
    Poly operator * (const Poly& b) const {
        std::vector<std::complex<double> > pa, pb;
        pa = FFT(v, deg()+b.deg()+1);
        pb = FFT(b.v, deg()+b.deg()+1);
        int n = pa.size();
        for(int i=0;i<n;++i)
            pa[i] *= pb[i];
        pa = FFT(pa, n);
        pb[0] = pa[0] / (double)n;
        for(int i=1;i<(int)pb.size();++i) {
            pb[i] = pa[n-i] / (double)n;
        }
        pb.resize(deg()+b.deg()+1);
        return pb;
    }
    friend std::ostream& operator << (std::ostream& stream, const Poly& poly) {
        for(auto x:poly.v) {
            stream << x ;
        }
        stream << std::endl;
        return stream;
    }
};

template<int64 M, int64 g>
class PolyMod {
private:
public:
    std::vector<Int<M> > v;
    int deg() const {return v.size()-1;}
    PolyMod():v(){}
    PolyMod(std::vector<Int<M> >&& vec) {
        v = std::move(vec);
    }
    PolyMod operator + (const PolyMod& b) const {
        PolyMod ans;
        ans.v.resize(std::max(deg()+1, b.deg()+1));
        for(int i=0;i<=ans.deg();++i)
            ans.v[i] = v[i] + b.v[i];
        return ans;
    }
    PolyMod operator - (const PolyMod& b) const {
        PolyMod ans;
        ans.v.resize(std::max(deg()+1, b.deg()+1));
        for(int i=0;i<=ans.deg();++i)
            ans.v[i] = v[i] - b.v[i];
        return ans;
    }
    PolyMod operator * (const PolyMod& b) const {
        std::vector<Int<M> > pa, pb;
        pa = NTT(v, deg()+b.deg()+1, g);
        pb = NTT(b.v, deg()+b.deg()+1, g);
        int n = pa.size();
        for(int i=0;i<n;++i)
            pa[i] *= pb[i];
        pa = NTT(pa, n, g);
        Int<M> n_inv = (Int<M>(n)^(M-2));
        pb[0] = pa[0] * n_inv;
        for(int i=1;i<(int)pb.size();++i) {
            pb[i] = pa[n-i] * n_inv;
        }
        pb.resize(deg()+b.deg()+1);
        return pb;
    }
    friend std::ostream& operator << (std::ostream& stream, const PolyMod& poly) {
        for(auto x:poly.v) {
            stream << x.val() << ' ' ;
        }
        stream << std::endl;
        return stream;
    }
};

char str[1000010];

int main() {
    PolyMod<998244353, 3> a, b;
    scanf("%s", str);
    int n = strlen(str);
    for(int i=n-1;i>=0;--i) {
        a.v.push_back(str[i] - '0');
    }
    scanf("%s", str);
    n = strlen(str);
    for(int i=n-1;i>=0;--i) {
        b.v.push_back(str[i] - '0');
    }
    // std::cout << a << b << std::endl;
    PolyMod<998244353, 3> c = a * b;
    std::vector<int> ans;
    int rest=0;
    for(int i=0;i<(int)c.v.size();++i) {
        int x = rest + c.v[i].val();
        ans.push_back(x%10);
        rest = x / 10;
    }
    if(rest)
        printf("%d", rest);
    for(auto it=ans.rbegin();it!=ans.rend();++it) {
        printf("%d", *it);
    }
    printf("\n");

    return 0;
}