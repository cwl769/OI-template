#include <cstdio>
#include <algorithm>
#include <vector>
typedef long long i64;
 
struct vector {
    i64 x, y;
    vector() : x(), y() {}
    vector(i64 _x, i64 _y) : x(_x), y(_y) {}
    vector operator + (const vector& b) const {
        return {x + b.x, y + b.y};
    }
    vector operator - (const vector& b) const {
        return {x - b.x, y - b.y};
    }
    vector operator * (const i64 k) const {
        return {x * k, y * k};
    }
};
vector operator * (const i64 k, const vector& v) {
    return {k * v.x, k * v.y};
}
i64 dot(const vector& a, const vector& b) {
    return a.x * b.x + a.y * b.y;   
}
i64 cro(const vector& a, const vector& b) {
    return a.x * b.y - a.y * b.x;
}
/* add this part to enable sort */
int which(const vector& b) {
    if (b.x == 0 && b.y == 0)
        return -1;
    int ans = 0;
    if (b.x && b.y) {
        ans |= 1;
        if (b.y < 0) ans |= 6;
        if (b.x < 0) ans ^= 2;
    }
    else {
        if ((b.x + b.y) < 0) ans |= 4;
        if (b.x == 0) ans |= 2;
    }
    return ans;
}
bool operator < (const vector& a, const vector& b) {
    if(which(a) == which(b))
        return cro(a, b) > 0;
    return which(a) < which(b);
}
/* add this part to enable convex */
void convex(std::vector<vector> &pnt, std::vector<vector> &ans) {
    std::sort(pnt.begin(), pnt.end(), [](const vector& a, const vector& b)->bool {
        if(a.x == b.x)
            return a.y < b.y;
        return a.x < b.x;
    });
    int n = pnt.size();
    std::vector<int> stack;
    std::vector<bool> vis(n);
    stack.emplace_back(0);
    for (int i = 1; i < n; ++i) {
        for(int sz = stack.size(); sz > 1; --sz) {
            const vector & a = pnt[stack[sz - 2]];
            const vector & b = pnt[stack[sz - 1]];
            if(cro(a - b, b - pnt[i]) > 0)
                break;
            stack.pop_back();
        }
        stack.emplace_back(i);
    }
    for (int i = 1; i < (int)stack.size(); ++i) {
        ans.emplace_back(pnt[stack[i]]);
        vis[stack[i]] = 1;
    }
    stack.clear();
    /* same as former one except the first element and the order */
    stack.emplace_back(n - 1);
    for (int i = n - 2; i >= 0; --i) {
        for(int sz = stack.size(); sz > 1; --sz) {
            const vector & a = pnt[stack[sz - 2]];
            const vector & b = pnt[stack[sz - 1]];
            if(cro(a - b, b - pnt[i]) > 0)
                break;
            stack.pop_back();
        }
        stack.emplace_back(i);
    }
    for (int i = 1; i < (int)stack.size(); ++i) {
        ans.emplace_back(pnt[stack[i]]);
        vis[stack[i]] = 1;
    }
}