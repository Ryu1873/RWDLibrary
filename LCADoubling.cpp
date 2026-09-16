#include<bits/stdc++.h>
#include <cassert>
using namespace std;
#define ll long long
const ll INF = 1'000'000'000'000'000'000LL;


// LCA(最小共通祖先)を高速に取得できるデータ構造
// コンストラクタ：LCATree tree(G, x) : Gは木、xは根。xを省略すると頂点0が根となる。前計算にO(NlogN)かかる。
// lca(u, v): uとvのLCAを求める。O(logN)
// dist(u, v): uとvの距離を求める。O(logN)
// on_path(u, x, v): u,vを結ぶパス上にxがあるかを判定する。O(logN)
struct LCATree {
private:
    ll N, log2_N;
    vector<vector<ll>> parent;
    vector<ll> depth;

    void dfs(vector<vector<ll>>& G, ll v, ll p, ll d) {
        parent[0][v] = p;
        depth[v] = d;
        for (ll u : G[v]) {
            if (u != p) dfs(G, u, v, d + 1);
        }
    }

public:
    LCATree(vector<vector<ll>>& G, ll root = 0) : N(G.size()) {
        log2_N = 1;
        while ((1 << log2_N) < N) log2_N++;
        parent.assign(log2_N, vector<ll>(N, -1));
        depth.assign(N, 0);
        dfs(G, root, -1, 0);
        for (ll k = 0; k + 1 < log2_N; k++) {
            for (ll v = 0; v < N; v++) {
                if (parent[k][v] < 0) {
                    parent[k + 1][v] = -1;
                } else {
                    parent[k + 1][v] = parent[k][parent[k][v]];
                }
            }
        }
    }

    ll lca(ll u, ll v) {
        assert(0 <= u && u < N && 0 <= v && v < N);
        if (depth[u] < depth[v]) swap(u, v);
        for (ll k = 0; k < log2_N; k++) {
            if (((depth[u] - depth[v]) >> k) & 1) {
                u = parent[k][u];
            }
        }
        if (u == v) return u;
        for (ll k = log2_N - 1; k >= 0; k--) {
            if (parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }
        return parent[0][u];
    }

    ll dist(ll u, ll v) {
        assert(0 <= u && u < N && 0 <= v && v < N);
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }

    bool on_path(ll u, ll x, ll v) {
        assert(0 <= u && u < N && 0 <= v && v < N && 0 <= x && x < N);
        return dist(u, x) + dist(v, x) == dist(u, v);
    }
};


int main() {
    
}