#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <algorithm>
#include <vector>
#include <iterator>
#include <list>
#include <forward_list>
#include <queue>

typedef long long int64;

template<typename T>
void readInt(T& x) {
    static bool f;
    x = 0;f = false;
    char c = getchar();
    for(;c<'0' || c>'9';c=getchar())if(c=='-')f=!f;
    for(;'0'<=c&&c<='9';c=getchar()) {
        x = x * 10 + c - '0';
    }
    if(f)x = -x;
}
template<typename T, typename ...Args>
void readInt(T& x, Args&... args) {
    readInt(x);
    readInt(args...);
}

template<typename T>
class Graph {
public:
    class Edge {
    public:
        Edge* next;
        int to;
        T val;
        Edge(Edge*_next, int _to, T _val):next(_next),to(_to),val(_val){}
    };
    class Iterator {
    private:
        Edge* ptr;
    public:
        Iterator(Edge *p):ptr(p){}
        Iterator& operator ++() {
            ptr = ptr->next;
            return *this;
        }
        const Iterator operator ++(int) {
            Edge * tmp = ptr;
            ptr = ptr->next;
            return tmp;
        }
        Edge& operator *() const {
            return *ptr;
        }
        bool operator != (const Iterator& b) {
            return this->ptr != b.ptr;
        }
    };
    class Edge_set {
    public:
        Edge *start;
        Edge_set():start(NULL){}
        Iterator begin(){
            return start;
        }
        Iterator end(){
            return NULL;
        }
    };
    Graph(int N):h(N) {}
    void add_edge(int x, int y, T v) {
        edg.push_front(Edge(h[x].start, y, v));
        h[x].start = &edg.front();
    }
    Edge_set operator () (int x) {
        return h[x];
    }
private:
    std::forward_list<Edge> edg;
    std::vector<Edge_set> h;
};

int main() {
    int n, m, s;readInt(n, m, s);
    Graph<int> g(n+1);
    for(int i=1;i<=m;++i) {
        int x, y, val;readInt(x, y, val);
        g.add_edge(x, y, val);
    }
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> >, std::greater<std::pair<int, int> > > heap;
    std::vector<int> dis(n+1, INT_MAX);
    std::vector<int> vis(n+1);
    dis[s] = 0;
    heap.push({dis[s], s});
    while(heap.size()) {
        int x = heap.top().second;heap.pop();
        if(vis[x])continue;
        vis[x] = true;
        for(auto e:g(x)) {
            int y = e.to;int val=e.val;
            if(dis[x] + val < dis[y]) {
                dis[y] = dis[x] + val;
                heap.push({dis[y], y});
            }
        }
    }
    for(int i=1;i<=n;++i) {
        printf("%d ", dis[i]);
    }

    return 0;
}