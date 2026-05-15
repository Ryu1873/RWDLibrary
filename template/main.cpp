#include<bits/stdc++.h>
#include<atcoder/all>
using namespace std;
using namespace atcoder;

// 型エイリアス
using ll = long long;
using mint = modint998244353;
using pll = pair<ll, ll>;
using tll = tuple<ll, ll, ll>;
using tll4 = tuple<ll, ll, ll, ll>;
using vll = vector<ll>; using vvll = vector<vll>; using vvvll =  vector<vvll>;
using vdl = vector<double>; using vvdl = vector<vdl>; using vvvdl = vector<vvdl>;
using vch = vector<char>; using vvch = vector<vch>; using vvvch = vector<vvch>;
using vst = vector<string>; using vvst = vector<vst>; using vvvst = vector<vvst>;
using vbo = vector<bool>; using vvbo = vector<vbo>; using vvvbo = vector<vvbo>;
using vmi = vector<mint>; using vvmi = vector<vmi>; using vvvmi = vector<vvmi>;
using vpll = vector<pll>; using vvpll = vector<vpll>; using vvvpll = vector<vvpll>;
using vtll = vector<tll>; using vvtll = vector<vtll>; using vvvtll = vector<vvtll>;
using vtll4 = vector<tll4>; using vvtll4 = vector<vtll4>; using vvvtll4 = vector<vvtll4>;

// rep系
#define rep(i, n) for(ll i=0;i<(n);i++)
#define rep1(i, n) for(ll i=1;i<=(n);i++)
#define rep3(i, m, n) for(ll i=(m);i<(n);i++)
#define rep4(i, m, n, d) for(ll i=(m);i<(n);i+=(d))
#define rrep(i, m, n) for(ll i=(m);i>=(n);i--)

// 入力
template<class T> void CIN(T& x) { cin >> x; }
template<class T, class... Ts> void CIN(T& x, Ts&... xs) { cin >> x; CIN(xs...); }
#define LL(...) ll __VA_ARGS__; CIN(__VA_ARGS__)
#define DBL(...) double __VA_ARGS__; CIN(__VA_ARGS__)
#define CHAR(...) char __VA_ARGS__; CIN(__VA_ARGS__)
#define STR(...) string __VA_ARGS__; CIN(__VA_ARGS__)
#define VEC(type, name, N) vector<type> name(N); for(auto& x: name) cin >> x
#define VEC2(type, name, N, M) vector<vector<type>> name(N, vector<type>(M)); for(auto& v : name) for(auto& x : v) cin >> x

// 出力
const vector<string> YESNO = {"Yes", "No"};
template<class T, class... Ts> void pr(const T& a, const Ts&... b) { cout << a; ((cout << ' ' << b), ...); cout << '\n'; }
template<class... Ts> void prs(const Ts&... a) { ((cout << a << ' '), ...); }
template<typename Container> void prvec(const Container& c) { bool first = true; for (const auto& x : c) {if (!first) {cout << ' ';} cout << x; first = false; } cout << '\n'; }
#define yes pr(YESNO[0])
#define no pr(YESNO[1])
inline bool yn(bool f) { if (f) { yes; return true; } no; return false; }

// その他便利マクロ
#define ALL(v) (v).begin(),(v).end()
#define RALL(v) (v).rbegin(),(v).rend()
template<class T> ll SIZE(T& v) { return v.size(); }
template<typename Container> void INC(Container& C) { for(const auto& x : C) x++; }
template<typename Container> void INC2(Container& C) { for(const auto& v : C) INC(v); }
template<typename Container> void DEC(Container& C) { for(const auto& x : C) x--; }
template<typename Container> void DEC2(Container& C) { for(const auto& v : C) DEC(v); }
template<typename Container> ll SUM(const Container& C) { ll ret = 0; for(const auto& x : C) { ret += x; } return ret; }
template<typename Container> ll SUM2(const Container& C) { ll ret = 0; for(const auto& v : C) { ret += SUM(v); } return ret; }
template<typename Container> ll MAXID(const Container& C) { return distance(C.begin(), max_element(C.begin(), C.end())); }
template<typename Container> ll MINID(const Container& C) { return distance(C.begin(), min_element(C.begin(), C.end())); }
template<typename Container, class T> ll FINDID(const Container& C, T x) { return distance(C.begin(), find(C.begin(), C.end(), x)); }
template<typename Container, class T> ll LBID(const Container& C, T x) { return distance(C.begin(), lower_bound(C.begin(), C.end(), x)); }
template<typename Container, class T> ll UBID(const Container& C, T x) { return distance(C.begin(), upper_bound(C.begin(), C.end(), x)); }
template<class T> bool CHMIN(T& a, T b){ if(a>b){ a=b; return true; } return false; }
template<class T> bool CHMAX(T& a, T b){ if(a<b){ a=b; return true; } return false; }
template<class... Ts> void SORT(Ts&... xs) { vector v = {xs...}; sort(v.begin(), v.end()); int i = 0; ((xs = v[i++]), ...); }
template<class T> bool INGRID(T x, T y, T H, T W) { return x >= 0 && x < H && y >= 0 && y < W; }

// 定数
const ll INF = 4'000'000'000'000'000'000LL;
const ll DI[] = {1, -1, 0, 0};
const ll DJ[] = {0, 0, 1, -1};
const string DIR = "DURL";

const bool IS_MULTI_CASE = false;


void solve() {
    
}


int main() {
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ll T = 1;
    if (IS_MULTI_CASE) cin >> T;
    while(T--) {
        solve();
    }
}