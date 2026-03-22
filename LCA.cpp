#include<bits/stdc++.h>
using namespace std;
#define ll long long
const ll INF = 1'000'000'000'000'000'000LL;

struct LCA {
    ll N;
    ll log2_N;
    // parent[k][v] := 頂点 v から 2^k 回親を辿った先の頂点 (存在しない場合は -1)
    vector<vector<ll>> parent;
    vector<ll> depth;

    // コンストラクタ: グラフと根の頂点番号を受け取り前計算を行う O(N log N)
    LCA(const vector<vector<ll>>& G, ll root = 0) : N(G.size()) {
        log2_N = 1;
        while ((1 << log2_N) < N) log2_N++;
        
        parent.assign(log2_N, vector<ll>(N, -1));
        depth.assign(N, 0);

        // 深さと1つ上の親を DFS で初期化
        dfs(G, root, -1, 0);

        // ダブリングによる前計算
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

    void dfs(const vector<vector<ll>>& G, ll v, ll p, ll d) {
        parent[0][v] = p;
        depth[v] = d;
        for (ll u : G[v]) {
            if (u != p) dfs(G, u, v, d + 1);
        }
    }

    // u と v の最小共通祖先を取得 O(log N)
    ll lca(ll u, ll v) const {
        // 常に u の方が深いようにする
        if (depth[u] < depth[v]) swap(u, v);

        // u と v の深さが同じになるまで u を遡らせる
        for (ll k = 0; k < log2_N; k++) {
            if (((depth[u] - depth[v]) >> k) & 1) {
                u = parent[k][u];
            }
        }

        // 深さを揃えた時点で一致していれば、それがLCA
        if (u == v) return u;

        // 二分探索の要領で、LCAの「1つ下の頂点」まで u, v を同時に遡らせる
        for (ll k = log2_N - 1; k >= 0; k--) {
            if (parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }

        // u (または v) の1つ上の頂点がLCA
        return parent[0][u];
    }

    // 2頂点間の距離（辺の数）を取得 O(log N)
    ll dist(ll u, ll v) const {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }
};


int main() {
    
}