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


// 繰り返し二乗法で累乗を計算．オーバーフローはしらん．
ll ipow(ll x, ll n) {
    ll ret = 1;
    while(n > 0) {
        if (n % 2 == 1) {
            ret = ret * x;
        }
        x *= x;
        n /= 2;
    }
    return ret;
}

// 平方根を計算．答えが整数にならない場合は切り捨て．
ll isqrt(ll n) {
    ll x = (ll)sqrt(n);
    while ((x + 1) * (x + 1) <= n) {
        x++;
    }
    while (x * x > n) {
        x--;
    }
    return x;
}



int main() {
    // 便利な標準関数
    ll x = 100;

    // 整数のpopcountを計算．
    ll cnt = __builtin_popcount(x);

    // 立っている整数の偶奇を返す．
    ll par = __builtin_parity(x);

    // 小さい方から何桁目に初めて1が現れるか(1-indexed)．
    ll ffs = __builtin_ffs(x);

    // 左側にいくつ0を埋める必要があるか．
    ll clz = __builtin_clz(x);

    // 1の位からいくつ0が連なっているか．
    ll ctz = __builtin_ctz(x);

    // 引数はどれもunsigned int．
    // undigned long longの場合はllをつける（例：__builtin_popcountll）
}