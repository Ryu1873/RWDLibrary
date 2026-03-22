#include<bits/stdc++.h>
#include<atcoder/all>
using namespace std;
using namespace atcoder;
#define ll long long
#define rep(i, n) for(ll i=0;i<(n);i++)
#define rep2(i, m, n) for(ll i=(m);i<(n);i++)
#define rrep(i, m, n) for(ll i=(m);i>=(n);i--)
#define all(v) (v).begin(),(v).end()
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

int main() {
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // シードの生成．
    random_device rd;

    // エンジンの初期化（メルセンヌ・ツイスタ）．
    // long longの範囲の乱数が必要ならmt19937_64．
    // シードを固定化させたい場合はrd()の部分に42とかを入れる．
    mt19937 eng(rd());
    mt19937_64 eng64(rd());

    // l以上r以下の整数を生成．
    ll l = 1, r = 100;
    uniform_int_distribution<ll> dist_int(l, r);
    ll rnum = dist_int(eng);

    // l2以上r2未満の実数を生成．
    double l2 = 0.0, r2 = 1.0;
    uniform_real_distribution<double> dist_real(l2, r2);
    double rnum2 = dist_real(eng);

    // 配列のシャッフル．
    vll v = {1, 2, 3, 4, 5};
    shuffle(all(v), eng);
}