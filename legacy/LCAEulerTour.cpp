#include<bits/stdc++.h>
#include <cassert>
using namespace std;
#define ll long long
const ll INF = 1'000'000'000'000'000'000LL;


// LCA(最小共通祖先)を高速に取得できるデータ構造．
// EulerTour+SparseTableでの実装（前計算O(NlogN)，クエリ当たりO(1)）．
// コンストラクタ：LCATree tree(N, root) (rootは省略すると0)
// add_edge(u, v)：辺(u,v)を追加．
// lca(u, v): uとvのLCAを求める。O(1)
// dist(u, v): uとvの距離を求める。O(1)
// on_path(u, x, v): u,vを結ぶパス上にxがあるかを判定する。O(1)
struct LCATree {
private:
    ll N, root;
    bool calced = false;
    vector<vector<ll>> G;
    
    vector<ll> euler; // EulerTourの頂点番号
    vector<ll> depth; // euler[i]の深さ
    vector<ll> first; // 頂点iがEulerTourで初めて現れる位置

    vector<vector<ll>> ST;
    vector<ll> lg;

    void dfs(ll v, ll p, ll d) {
        first[v] = euler.size();
        euler.push_back(v);
        depth.push_back(d);
        for (ll u : G[v]) {
            if (u == p) continue;
            dfs(u, v, d + 1);
            euler.push_back(v);
            depth.push_back(d);
        }
    }

    void calc() {
        calced = true;
        dfs(root, -1, 0);
        ll M = euler.size();
        lg.resize(M+1);
        for (ll i = 2; i <= M; i++) {
            lg[i] = lg[i/2] + 1;
        }

        ll K = lg[M] + 1;
        ST.assign(K, vector<ll>(M));
        iota(ST[0].begin(), ST[0].end(), 0);
        for (ll k = 1; k < K; k++) {
            for (ll i = 0; i + (1 << k) <= M; i++) {
                ll x = ST[k-1][i];
                ll y = ST[k-1][i + (1 << (k-1))];
                ST[k][i] = (depth[x] < depth[y] ? x : y);
            }
        }
    }

public:
    LCATree(ll N, ll root = 0) : N(N), root(root), G(N), first(N, -1) {}

    void add_edge(ll u, ll v) {
        assert(!calced && 0 <= u && u < N && 0 <= v && v < N);
        G[u].push_back(v);
        G[v].push_back(u);
    }

    ll lca(ll u, ll v) {
        assert(0 <= u && u < N && 0 <= v && v < N);
        if (!calced) calc();
        ll l = first[u], r = first[v];
        if (l > r) swap(l, r);
        ll k = lg[r - l + 1];
        ll x = ST[k][l], y = ST[k][r - (1 << k) + 1];
        return depth[x] < depth[y] ? euler[x] : euler[y];
    }

    ll dist(ll u, ll v) {
        assert(0 <= u && u < N && 0 <= v && v < N);
        if (!calced) calc();
        return depth[first[u]] + depth[first[v]] - 2 * depth[first[lca(u, v)]];
    }

    bool on_path(ll u, ll x, ll v) {
        assert(0 <= u && u < N && 0 <= v && v < N && 0 <= x && x < N);
        if (!calced) calc();
        return dist(u, x) + dist(v, x) == dist(u, v);
    }
};


int main() {
    
}