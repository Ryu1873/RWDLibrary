#include<bits/stdc++.h>
#include<atcoder/all>
using namespace std;
using namespace atcoder;
#define ll long long
#define rep(i, n) for(ll i=0;i<(n);i++)
#define rep2(i, m, n) for(ll i=(m);i<(n);i++)
#define rrep(i, m, n) for(ll i=(m);i>=(n);i--)
#define all(v) (v).begin(),(v).end()
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
void CIN(){}
template <class Head,class... Tail>
void CIN(Head&& head,Tail&&... tail) {cin>>head; CIN(move(tail)...);}
#define llin(...) ll __VA_ARGS__;CIN(__VA_ARGS__)
#define stin(...) string __VA_ARGS__;CIN(__VA_ARGS__)
#define cinv(x) for(auto&youso_: (x) )cin>>youso_
#define say(x) cout<<(x)<<endl
#define says(x) cout<<(x)<<" "
#define sayv(x) for(auto&youso_: (x) )cout<<youso_<<" ";cout<<endl
#define YES(n) cout << ((n) ? "YES" : "NO") << endl
#define Yes(n) cout << ((n) ? "Yes" : "No") << endl
template<class T> inline bool chmin(T& a, T b){ if(a>b){ a=b; return true; } return false; }
template<class T> inline bool chmax(T& a, T b){ if(a<b){ a=b; return true; } return false; }
const ll INF = 1'000'000'000'000'000'000LL;

int main() {
    // N頂点0辺のグラフを作成する．<ll>はcapの型．
    ll N;
    mf_graph<ll> G(N);

    // fromからtoへ最大容量cap，流量0の辺を追加する．
    // 返り値は何番目に追加された辺であるか．
    ll from = 0, to = N-1, cap = 100;
    G.add_edge(from, to, cap);

    // 頂点sからtへ可能な限り流し，流せた量を返す．
    // 時間計算量はO(n^2m)．辺の容量が全て1ならO(min(n^(2/3)m, m^(3/2)))．
    ll s = 0, t = N-1;
    ll flow = G.flow(s, t);

    // limitに達するまで流す．
    ll limit = 10;
    flow = G.flow(s, t, limit);

    // sからiへ残余グラフで到達可能なときのみtrueとなるようなvectorを返す．
    // flow(s, t)をちょうど1回呼んだあとに呼ぶと，返り値はs,t間のmincutに対応する．
    // 時間計算量はO(n+m)．
    vbo mc = G.min_cut(s);

    // i番目の辺を取得する．
    ll i = 0;
    mf_graph<ll>::edge edge = G.get_edge(i);

    // 全辺を取得．
    vector<mf_graph<ll>::edge> edges = G.edges();

    // 辺は以下のメンバを持つ構造体．
    from = edge.from;
    to = edge.to;
    cap = edge.cap;
    flow = edge.flow;

    // i番目の辺の容量，流量を変更する．
    ll new_cap = 1000, new_flow = 500;
    G.change_edge(i, new_cap, new_flow);
}