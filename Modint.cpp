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

// modが固定の場合．
// using mint = modint998244353;
// using mint = modint1000000007;
// using mint = static_modint<1000000000>;

// modが固定でない場合．
using mint = modint;

int main() {
    // modが固定でない場合にmodを設定する．
    mint::set_mod(10000);

    // コンストラクタ．初期値は0．
    mint xx();

    // 初期値を設定できるコンストラクタ．
    ll y = 100;
    mint x(y);

    // modを取得する．
    int m = mint::mod();

    // 格納されている値を取得する．
    int v = x.val();

    // 定義されている演算．
    // + - * / += -= *= /= ++ -- == !=
    // a/bを行うとき，gcd(b,mod)=1である必要がある．
    // 割り算のみO(log mod)時間，それ以外はO(1)時間．

    // x^nを計算．O(logn).
    ll n = 10;
    mint p = x.pow(n);

    // xの逆元を計算．gcd(x,mod)=1である必要がある．O(log mod)．
    mint i = x.inv();

    // 定数倍高速化．addがmod未満であることがわかっているとき，下の方が速い．
    int add = 1;
    x += add;
    x += mint::raw(add);
}