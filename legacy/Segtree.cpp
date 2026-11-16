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


// 二項演算を定義．モノイドである，すなわち結合律を満たし，単位元が存在する必要がある．
ll op(ll a, ll b) {
    return max(a, b);
}


// 単位元を定義.
ll e() {
    return 0ll;
}


int main() {
    // 長さNのセグ木を作る．初期値は全て0．
    // 計算量：O(n)
    ll N = 100;
    segtree<ll, op, e> seg(N);


    // vectorの要素を初期値とするセグ木を作る．
    // 計算量：O(n)
    vll vec(N, 0);
    segtree<ll, op, e> seg(vec);


    // i番目の要素にxを代入する．
    // 計算量：O(logn)
    ll i = 10, x = 1000;
    seg.set(i, x);


    // i番目の要素を取得する．
    // 計算量：O(1)
    seg.get(i);
    

    // l番目からr-1番目までをopに基づいて計算する．l=rなら単位元を返す．
    // 計算量：O(logn)
    ll l = 5, r = 15;
    ll ans = seg.prod(l, r);


    // 全体をopに基づいて計算する．
    // 計算量：O(1)
    ans = seg.all_prod();


    // max_right,min_left
    // セグ木上の二分探索．後で調べる．
}