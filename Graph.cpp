#include<bits/stdc++.h>
#include<atcoder/all>
using namespace std;
using namespace atcoder;
#define ll long long
#define rep(i, n) for(ll i=0;i<(n);i++)
#define rep1(i, n) for(ll i=1;i<=(n);i++)
#define rep3(i, m, n) for(ll i=(m);i<(n);i++)
#define rrep(i, m, n) for(ll i=(m);i>=(n);i--)
#define all(v) (v).begin(),(v).end()
#define rall(v) (v).rbegin(),(v).rend()
#define pll pair<ll, ll>
#define vll vector<ll>
#define vvll vector<vector<ll>>
#define vdl vector<double>
#define vvdl vector<vector<double>>
#define vch vector<char>
#define vvch vector<vector<char>>
#define vst vector<string>
#define vvst vector<vector<string>>
#define vbo vector<bool>
#define vvbo vector<vector<bool>>
#define vpll vector<pll>
#define vvpll vector<vector<pll>>
void CIN(){}
template <class Head,class... Tail>
void CIN(Head&& head,Tail&&... tail) {cin>>head; CIN(move(tail)...);}
#define llin(...) ll __VA_ARGS__;CIN(__VA_ARGS__)
#define stin(...) string __VA_ARGS__;CIN(__VA_ARGS__)
#define cinv(x) for(auto&youso_: (x) )cin>>youso_
#define say(x) cout<<(x)<<"\n"
#define says(x) cout<<(x)<<" "
#define sayv(x) for(auto&youso_: (x) )cout<<youso_<<" ";cout<<"\n"
#define YES(n) cout << ((n) ? "YES" : "NO") << "\n"
#define Yes(n) cout << ((n) ? "Yes" : "No") << "\n"
template<class T> inline bool chmin(T& a, T b){ if(a>b){ a=b; return true; } return false; }
template<class T> inline bool chmax(T& a, T b){ if(a<b){ a=b; return true; } return false; }
const ll INF = 1'000'000'000'000'000'000LL;
using mint = modint998244353;


ll N;


// vから各頂点への到達可能性を調べるシンプルなDFS．
void dfs(ll v, vvll &G, vbo &vis) {
    vis[v] = true;
    for (ll u : G[v]) {
        if (!vis[u]) {
            dfs(u, G, vis);
        }
    }
}


// BFSで頂点sから各地点までの距離を求める．到達不可能な頂点は距離INFとする．
vector<ll> BFS(ll s, vector<vector<ll>>& G) {
    vector<ll> dist(G.size(), INF);
    queue<ll> que;
    dist[s] = 0;
    que.push(s);
    while(!que.empty()) {
        ll v = que.front();
        que.pop();
        for (ll u : G[v]) {
            if (dist[u] == INF) {
                dist[u] = dist[v] + 1;
                que.push(u);
            }
        }
    }
    return dist;
}


// 多始点BFS．
vector<ll> multiStartBFS(vector<ll>& starts, vector<vector<ll>>& G) {
    vector<ll> dist(G.size(), INF);
    queue<ll> que;
    for (ll v : starts) {
        dist[v] = 0;
        que.push(v);
    }
    while(!que.empty()) {
        ll v = que.front();
        que.pop();
        for (ll u : G[v]) {
            if (dist[u] == INF) {
                dist[u] = dist[v] + 1;
                que.push(u);
            }
        }
    }
    return dist;
}

// 01-BFS．
// G：隣接リスト．要素は{辺のコスト(0|1)，隣接頂点}
vector<ll> BFS01(ll s, vector<vector<pair<ll, ll>>>& G) {
    vector<ll> dist(G.size(), INF);
    deque<ll> deq;
    dist[s] = 0;
    deq.push_back(s);
    while(!deq.empty()) {
        ll v = deq.front();
        deq.pop_front();
        for (auto p : G[v]) {
            ll c, u; tie(c, u) = p;
            if (dist[u] > dist[v] + c) {
                dist[u] = dist[v] + c;
                if (c == 0) {
                    deq.push_front(u);
                } else {
                    deq.push_back(u);
                }
            }
        }
    }
    return dist;
}

// 多始点01-BFS．
vector<ll> multiStartBFS01(vector<ll>& starts, vector<vector<pair<ll, ll>>>& G) {
    vector<ll> dist(G.size(), INF);
    deque<ll> deq;
    for (ll v : starts) {
        dist[v] = 0;
        deq.push_back(v);
    }
    while(!deq.empty()) {
        ll v = deq.front();
        deq.pop_front();
        for (auto p : G[v]) {
            ll c, u; tie(c, u) = p;
            if (dist[u] > dist[v] + c) {
                dist[u] = dist[v] + c;
                if (c == 0) {
                    deq.push_front(u);
                } else {
                    deq.push_back(u);
                }
            }
        }
    }
    return dist;
}


// 木Gの直径を求める．
// 返り値：{直径，直径の端点1，直径の端点2}．
tuple<ll, ll, ll> treeDiameter(vector<vector<ll>>& G) {
    vector<ll> dist = BFS(0, G);
    ll max_idx = distance(dist.begin(), max_element(dist.begin(), dist.end()));
    dist = BFS(max_idx, G);
    ll max_idx2 = distance(dist.begin(), max_element(dist.begin(), dist.end()));
    return {dist[max_idx2], max_idx, max_idx2};
}


// 重み付きの木Gの直径を求める．
// 返り値：{直径，直径の端点1，直径の端点2}．
tuple<ll, ll, ll> weightedTreeDiameter(vector<vector<pll>>& G) {
    vector<ll> dist(G.size(), INF);
    auto dfs = [&](auto&& dfs, ll v, ll p, ll w) -> void {
        dist[v] = w;
        for (auto [u, x] : G[v]) {
            if (u == p) continue;
            dfs(dfs, u, v, w+x);
        }
    };
    dfs(dfs, 0, -1, 0);
    ll max_idx = distance(dist.begin(), max_element(dist.begin(), dist.end()));
    fill(dist.begin(), dist.end(), INF);
    dfs(dfs, max_idx, -1, 0);
    ll max_idx2 = distance(dist.begin(), max_element(dist.begin(), dist.end()));
    return {dist[max_idx2], max_idx, max_idx2};
}


// sから各点への最短距離を求める．
// G：隣接リスト．要素は{辺のコスト，隣接頂点}
vector<ll> dijkstra(vector<vector<pair<ll, ll>>>& G, ll s = 0) {
    vector<ll> dist(G.size(), INF);
    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pll>> pq;
    pq.emplace(0, s);
    while(!pq.empty()) {
        ll c, v;
        tie(c, v) = pq.top();
        pq.pop();
        if (dist[v] != INF) continue;
        dist[v] = c;
        for (auto p : G[v]) {
            ll d, u;
            tie(d, u) = p;
            if (dist[u] == INF) {
                pq.emplace(c + d, u);
            }
        }
    }
    return dist;
}


// ベルマン・フォード法
// 重みつきグラフにおいて，頂点sから各頂点への距離をO(N^2 + M)で求める．
// Gはグラフの隣接リスト(重み，次の頂点の順)
// 到達不可能な頂点はINF，負閉路がある場合は全頂点に-INFを格納．
vector<ll> bellmanFord(vector<vector<pair<ll, ll>>>& G, ll s = 0) {
    ll N = G.size();
    vector<ll> D(N, INF);
    D[s] = 0;
    for (ll t = 0; t < N; t++) {
        bool update = false;
        for (ll v = 0; v < N; v++) {
            for (auto [w, u] : G[v]) {
                if (D[v] + w < D[u]) {
                    D[u] = D[v] + w;
                    update = true;
                }
            }
        }
        if (!update) {
            break;
        } else if (t == N-1) {
            fill(D.begin(), D.end(), -INF);
        }
    }
    return D;
}


int main() {
    
}