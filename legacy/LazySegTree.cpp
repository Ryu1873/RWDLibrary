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

// 区間和取得+区間更新(modint版)
struct S { mint val; ll size; };
using F = optional<mint>;
S e() { return {0, 0}; }
F id() { return nullopt; }
S op(S a, S b) { return {a.val + b.val, a.size + b.size}; }
S mapping(F f, S x) { 
    if (f) return {*f * x.size, x.size};
    return x; 
}
F composition(F f, F g) { return f ? f : g; }


int main() {
    lazy_segtree<S, op, e, F, mapping, composition, id> seg(100);
}