#include<bits/stdc++.h>
#include<atcoder/all>
using namespace std;
using namespace atcoder;

// 型エイリアス
using ll = long long;
using i128 = __int128_t;
using ldb = long double;
using mint = modint998244353;
using pll = pair<ll, ll>;
using tll = tuple<ll, ll, ll>;
using tll4 = tuple<ll, ll, ll, ll>;
using vll = vector<ll>; using vvll = vector<vll>; using vvvll =  vector<vvll>;
using vld = vector<ldb>; using vvld = vector<ldb>; using vvvdl = vector<vvld>;
using vch = vector<char>; using vvch = vector<vch>; using vvvch = vector<vvch>;
using vst = vector<string>; using vvst = vector<vst>; using vvvst = vector<vvst>;
using vbo = vector<bool>; using vvbo = vector<vbo>; using vvvbo = vector<vvbo>;
using vmi = vector<mint>; using vvmi = vector<vmi>; using vvvmi = vector<vvmi>;
using vpll = vector<pll>; using vvpll = vector<vpll>; using vvvpll = vector<vvpll>;
using vtll = vector<tll>; using vvtll = vector<vtll>; using vvvtll = vector<vvtll>;
using vtll4 = vector<tll4>; using vvtll4 = vector<vtll4>; using vvvtll4 = vector<vvtll4>;

// 補助マクロ
#define OVERLOAD4(a, b, c, d, e, ...) e
#define OVERLOAD6(a, b, c, d, e, f, g, ...) g

// rep系
#define REP1(n) for(ll _ = 0 ; _ < (n); _++)
#define REP2(i, n) for(ll i = 0; i < (n); i++)
#define REP3(i, m, n) for(ll i = (m); i < (n); i++)
#define REP4(i, m, n, d) for(ll i = (m); i < (n); i += (d))
#define RREP1(n) for(ll _ = (n) - 1 ; _ >= 0; _--)
#define RREP2(i, n) for(ll i = (n) - 1; i >= 0; i--)
#define RREP3(i, m, n) for(ll i = (m) - 1; i >= (n); i--)
#define RREP4(i, m, n, d) for(ll i = (m) - 1; i >= (n); i -= (d))
#define rep(...) OVERLOAD4(__VA_ARGS__, REP4, REP3, REP2, REP1)(__VA_ARGS__)
#define rrep(...) OVERLOAD4(__VA_ARGS__, RREP4, RREP3, RREP2, RREP1)(__VA_ARGS__)

// 入力
template<class T> void CIN(T& x) { cin >> x; }
template<class T, class... Ts> void CIN(T& x, Ts&... xs) { cin >> x; CIN(xs...); }
#define LL(...) ll __VA_ARGS__; CIN(__VA_ARGS__)
#define LDB(...) ldb __VA_ARGS__; CIN(__VA_ARGS__)
#define CHAR(...) char __VA_ARGS__; CIN(__VA_ARGS__)
#define STR(...) string __VA_ARGS__; CIN(__VA_ARGS__)
#define VEC3(type, name, N) vector<type> name(N); for(auto& x: name) cin >> x
#define VEC4(type, name1, name2, N) vector<type> name1(N), name2(N); for(ll i = 0; i < N; i++) cin >> name1[i] >> name2[i]
#define VEC5(type, name1, name2, name3, N) vector<type> name1(N), name2(N), name3(N); for(ll i = 0; i < N; i++) cin >> name1[i] >> name2[i] >> name3[i]
#define VEC6(type, name1, name2, name3, name4, N) vector<type> name1(N), name2(N), name3(N), name4(N); for(ll i = 0; i < N; i++) cin >> name1[i] >> name2[i] >> name3[i] >> name4[i]
#define VEC(...) OVERLOAD6(__VA_ARGS__, VEC6, VEC5, VEC4, VEC3)(__VA_ARGS__)
#define VLL(...) VEC(ll, __VA_ARGS__)
#define VLD(...) VEC(ldb, __VA_ARGS__)
#define VST(...) VEC(string, __VA_ARGS__)
#define VVEC4(type, name, N, M) vector<vector<type>> name(N, vector<type>(M)); for(auto& v : name) for(auto& x : v) cin >> x
#define VVEC3(type, name, N) VVEC4(type, name, N, N)
#define VVEC(...) OVERLOAD4(__VA_ARGS__, VVEC4, VVEC3)(__VA_ARGS__)
#define VVLL(...) VVEC(ll, __VA_ARGS__)

// 出力
const pair<string, string> YESNO_STRINGS = {"Yes", "No"};
template<class T, class... Ts> void PR(const T& a, const Ts&... b) { cout << a; ((cout << ' ' << b), ...); cout << '\n'; }
template<class... Ts> void PRS(const Ts&... a) { ((cout << a << ' '), ...); }
template<typename Container> void PRC(const Container& c) { bool first = true; for (const auto& x : c) {if (!first) {cout << ' ';} cout << x; first = false; } cout << '\n'; }
#define YES PR(YESNO_STRINGS.first)
#define NO PR(YESNO_STRINGS.second)
bool YN(bool f) { if (f) { YES; return true; } NO; return false; }

// 整数系マクロ
template<typename T, typename U> T MOD(T n, U m) { return (n % m + m) % m; }
template<typename T> bool ODD(T n) { return MOD(n, 2) == 1; }
template<typename T> bool EVEN(T n) { return MOD(n, 2) == 0; }
template<typename T, typename U> T CEIL(T n, U d) { return d < 0 ? CEIL(-n, -d) : (n >= 0 ? (n + d - 1) / d : n / d); }
template<typename T, typename U> T FLOOR(T n, U d) { return d < 0 ? FLOOR(-n, -d) : (n >= 0 ? n / d : (n + d - 1) / d); }
template<typename T, typename U> T POW(T x, U n) { assert(n >= 0); T ret = 1; while(n > 0) { if (n % 2 == 1) { ret = ret * x; } x *= x; n /= 2; } return ret; }
template<typename T> T SQRT(T n) { T x = (T)sqrt(n); while((x + 1) * (x + 1) <= n) { x++; } while(x * x > n) { x--; } return x; }
template<typename T, typename U> bool BIT(T n, U i) { return (n >> i) & 1; }
template<typename T> ll POPCNT(T n) { return popcount((unsigned long long)n); }
template<typename T> ll DIGSZ(T n) { return to_string(n).size(); }


// コンテナ系マクロ
#define ALL(v) (v).begin(),(v).end()
#define RALL(v) (v).rbegin(),(v).rend()
#define SZ(v) (ll)(v).size()
template<typename T, typename U> void ADD(T& v, const U& x) { if constexpr (ranges::range<T>) { for (auto& e : v) { ADD(e, x); } } else { v += x; } }
template<typename Container> void INC(Container& C) { ADD(C, 1); }
template<typename Container> void DEC(Container& C) { ADD(C, -1); }
template<typename T> void UNIQUE(T& A) { sort(A.begin(), A.end()); A.erase(unique(A.begin(), A.end()), A.end()); }
template<typename T> auto SUM(const T& v) { if constexpr (ranges::range<T>) { decltype(SUM(*begin(v))) ret = 0; for (const auto& e : v) { ret += SUM(e); } return ret; } else { return v; } }
template<typename Container> ll MAXID(const Container& C) { return distance(C.begin(), max_element(C.begin(), C.end())); }
template<typename Container> ll MINID(const Container& C) { return distance(C.begin(), min_element(C.begin(), C.end())); }
template<typename Container, class T> ll FINDID(const Container& C, T x) { return distance(C.begin(), find(C.begin(), C.end(), x)); }
template<typename Container, class T> ll LBID(const Container& C, T x) { return distance(C.begin(), lower_bound(C.begin(), C.end(), x)); }
template<typename Container, class T> ll UBID(const Container& C, T x) { return distance(C.begin(), upper_bound(C.begin(), C.end(), x)); }
template<typename Container> auto RLE(const Container& C) { vector<pair<typename Container::value_type, ll>> ret; for (const auto& x : C) { if (ret.empty() || x != ret.back().first) { ret.emplace_back(x, 1); } else { ret.back().second++; } } return ret; }
template<typename T> vector<T> CUM(const vector<T>& V) { vector<T> ret(V.size()+1, 0); for (ll i = 0; i < (ll)V.size(); i++) { ret[i+1] = ret[i] + V[i]; } return ret; }

// 幾何系マクロ
template<typename T> bool INGRID(T x, T y, T H, T W) { return x >= 0 && x < H && y >= 0 && y < W; }
template<typename C> vector<C> TRANSPOSE(vector<C>& G) { vector<C> H(G[0].size(), C(G.size(), 0)); for(ll i = 0; i < (ll)G[0].size(); i++) for(ll j = 0; j < (ll)G.size(); j++) H[i][j] = G[j][i]; return H; }
template<typename T> T EDIST(T x1, T y1, T x2, T y2) { return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); }
template<typename T> T MDIST(T x1, T y1, T x2, T y2) { return abs(x1 - x2) + abs(y1 - y2); }

// その他便利マクロ
template<typename T, typename U> bool CHMIN(T& a, U b){ if(a>b){ a=b; return true; } return false; }
template<typename T, typename U> bool CHMAX(T& a, U b){ if(a<b){ a=b; return true; } return false; }
template<typename... Ts> void SORT(Ts&... xs) { vector v = {xs...}; sort(v.begin(), v.end()); int i = 0; ((xs = v[i++]), ...); }

// 定数
const ll INF = 4'000'000'000'000'000'000LL;
const ll DI[] = {-1, 0, 1, 0};
const ll DJ[] = {0, 1, 0, -1};
const ll DI8[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll DJ8[] = {0, 1, 1, 1, 0, -1, -1, -1};
const string DIR = "URDL";

const bool IS_MULTI_CASE = false;


void solve() {
    
}


int main() {
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    cout << fixed << setprecision(15);

    ll T = 1;
    if (IS_MULTI_CASE) cin >> T;
    while(T--) {
        solve();
    }
}
