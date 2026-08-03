#include <cstdio>
#include <cmath>
#include <cstring>
#include <vector>
#include <complex>
#include <iostream>

typedef long long i64;
typedef unsigned long long ui64;
const double pi = acos(-1);

int log2(int x) {
    int ans = -1;
    for(;x;x>>=1)++ans;
    return ans;
}

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