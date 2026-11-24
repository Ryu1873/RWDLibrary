// https://github.com/Ryu1873/RWDLibrary

#include<bits/stdc++.h>
#include<cassert>
#if __has_include(<atcoder/all>)
#include<atcoder/all>
using namespace atcoder;
#endif
using namespace std;

// 型エイリアス
using ll = long long;
using i128 = __int128_t;
using ldb = long double;
#if __has_include(<atcoder/all>)
using mint = modint998244353;
#endif
using pll = pair<ll, ll>;
using tll = tuple<ll, ll, ll>;
using tll4 = tuple<ll, ll, ll, ll>;
using vll = vector<ll>; using vvll = vector<vll>; using vvvll =  vector<vvll>;
using vld = vector<ldb>; using vvld = vector<vld>; using vvvld = vector<vvld>;
using vch = vector<char>; using vvch = vector<vch>; using vvvch = vector<vvch>;
using vst = vector<string>; using vvst = vector<vst>; using vvvst = vector<vvst>;
using vbo = vector<bool>; using vvbo = vector<vbo>; using vvvbo = vector<vvbo>;
#if __has_include(<atcoder/all>)
using vmi = vector<mint>; using vvmi = vector<vmi>; using vvvmi = vector<vvmi>;
#endif
using vpll = vector<pll>; using vvpll = vector<vpll>; using vvvpll = vector<vvpll>;
using vtll = vector<tll>; using vvtll = vector<vtll>; using vvvtll = vector<vvtll>;
using vtll4 = vector<tll4>; using vvtll4 = vector<vtll4>; using vvvtll4 = vector<vvtll4>;

// 補助マクロ
#define CAT_IMPL(a, b) a##b
#define CAT(a, b) CAT_IMPL(a, b)
#define OVERLOAD4(a, b, c, d, e, ...) e

// rep系
#define REP1_IMPL(i, n) for(ll i = 0; i < (n); i++)
#define REP1(n) REP1_IMPL(CAT(_rep_, __COUNTER__), n)
#define REP2(i, n) for(ll i = 0; i < (n); i++)
#define REP3(i, m, n) for(ll i = (m); i < (n); i++)
#define REP4(i, m, n, d) for(ll i = (m); i < (n); i += (d))
#define RREP1_IMPL(i, n) for(ll i = (n) - 1; i >= 0; i--)
#define RREP1(n) RREP1_IMPL(CAT(_rep_, __COUNTER__), n)
#define RREP2(i, n) for(ll i = (n) - 1; i >= 0; i--)
#define RREP3(i, m, n) for(ll i = (m) - 1; i >= (n); i--) // [n, m) を降順に走査
#define RREP4(i, m, n, d) for(ll i = (m) - 1; i >= (n); i -= (d)) // [n, m) を d 刻みで降順に走査
#define rep(...) OVERLOAD4(__VA_ARGS__, REP4, REP3, REP2, REP1)(__VA_ARGS__)
#define rrep(...) OVERLOAD4(__VA_ARGS__, RREP4, RREP3, RREP2, RREP1)(__VA_ARGS__)

// 入力
template<class T> void CIN(T& x) { cin >> x; }
template<class T, class... Ts> void CIN(T& x, Ts&... xs) { cin >> x; CIN(xs...); }
#define LL(...) ll __VA_ARGS__; CIN(__VA_ARGS__)
#define LDB(...) ldb __VA_ARGS__; CIN(__VA_ARGS__)
#define CHAR(...) char __VA_ARGS__; CIN(__VA_ARGS__)
#define STR(...) string __VA_ARGS__; CIN(__VA_ARGS__)
template<class... Ts> void READ_VECS(ll n, Ts&... vs) { (vs.resize(n), ...); for (ll i = 0; i < n; i++) (cin >> ... >> vs[i]); }
#define VEC(type, N, ...) vector<type> __VA_ARGS__; READ_VECS(N, __VA_ARGS__)
#define VLL(...) VEC(ll, __VA_ARGS__)
#define VLD(...) VEC(ldb, __VA_ARGS__)
#define VST(...) VEC(string, __VA_ARGS__)
template<class T> void READ_VVEC(ll n, ll m, vector<vector<T>>& a) { a.assign(n, vector<T>(m)); for (auto& v : a) for (auto& x : v) cin >> x; }
template<class T> void READ_VVEC(ll n, vector<vector<T>>& a) { READ_VVEC(n, n, a); }
#define VVEC4(type, N, M, name) vector<vector<type>> name; READ_VVEC(N, M, name)
#define VVEC3(type, N, name) vector<vector<type>> name; READ_VVEC(N, name)
#define VVEC(...) OVERLOAD4(__VA_ARGS__, VVEC4, VVEC3)(__VA_ARGS__)
#define VVLL(...) VVEC(ll, __VA_ARGS__)
template<bool directed> void READ_GRAPH(ll n, ll m, vvll& g) { g.assign(n, {}); for (ll i = 0; i < m; i++) { ll u, v; cin >> u >> v; u--; v--; g[u].push_back(v); if constexpr (!directed) g[v].push_back(u); } }
template<bool directed> void READ_WGRAPH(ll n, ll m, vvpll& g) { g.assign(n, {}); for (ll i = 0; i < m; i++) { ll u, v, w; cin >> u >> v >> w; u--; v--; g[u].emplace_back(v, w); if constexpr (!directed) g[v].emplace_back(u, w); } }
void READ_TREE(ll n, vvll& g) { READ_GRAPH<false>(n, n - 1, g); }
#define UGRAPH(N, M, G) vvll G; READ_GRAPH<false>(N, M, G)
#define DGRAPH(N, M, G) vvll G; READ_GRAPH<true>(N, M, G)
#define WUGRAPH(N, M, G) vvpll G; READ_WGRAPH<false>(N, M, G)
#define WDGRAPH(N, M, G) vvpll G; READ_WGRAPH<true>(N, M, G)
#define TREE(N, G) vvll G; READ_TREE(N, G)

// 出力
const pair<string, string> YESNO_STRINGS = {"Yes", "No"};
template<class T, class... Ts> void PR(const T& a, const Ts&... b) { cout << a; ((cout << ' ' << b), ...); cout << '\n'; }
template<class... Ts> void PRS(const Ts&... a) { ((cout << a << ' '), ...); }
template<typename Container> void PRC(const Container& c) { bool first = true; for (const auto& x : c) {if (!first) {cout << ' ';} cout << x; first = false; } cout << '\n'; }
#define YES PR(YESNO_STRINGS.first)
#define NO PR(YESNO_STRINGS.second)
bool YN(bool f) { if (f) { YES; return true; } NO; return false; }

// 整数系マクロ
ll MOD(ll n, ll m) { assert(m > 0); ll r = n % m; return r < 0 ? r + m : r; }
bool ODD(ll n) { return (n & 1) != 0; }
bool EVEN(ll n) { return (n & 1) == 0; }
ll CEIL(ll n, ll d) { assert(d != 0); ll q = n / d, r = n % d; return q + (r != 0 && ((r < 0) == (d < 0))); }
ll FLOOR(ll n, ll d) { assert(d != 0); ll q = n / d, r = n % d; return q - (r != 0 && ((r < 0) != (d < 0))); }
ll POW(ll x, ll n) { assert(n >= 0); ll ret = 1; while (n > 0) { if (n & 1) { ret *= x; } n >>= 1; if (n > 0) { x *= x; } } return ret; }
ll FLOG(ll n, ll x) { assert(n >= 2 && x >= 1); ll ret = 0; while (x >= n) { x /= n; ret++; } return ret; }
ll CLOG(ll n, ll x) { assert(n >= 2 && x >= 1); return x == 1 ? 0 : FLOG(n, x - 1) + 1; }
ll SQRT(ll n) { assert(n >= 0); if (n == 0) return 0; ll x = (ll)sqrtl((ldb)n); while (x + 1 <= n / (x + 1)) { x++; } while (x > n / x) { x--; } return x; }
bool BIT(ll n, ll i) { assert(0 <= i && i < 64); return ((unsigned long long)n >> i) & 1; }
ll POPCNT(ll n) { return popcount((unsigned long long)n); }
ll MSB(ll n) { return bit_width((unsigned long long)n); } // 1-indexed(開区間)
ll DIGSZ(ll n) { ll ret = 1; while (n <= -10 || n >= 10) { n /= 10; ret++; } return ret; }


// コンテナ系マクロ
#define ALL(v) (v).begin(),(v).end()
#define RALL(v) (v).rbegin(),(v).rend()
#define SZ(v) ((ll)(v).size())
template<typename T, typename U> void ADD(T& v, const U& x) { if constexpr (ranges::range<T>) { for (auto& e : v) { ADD(e, x); } } else { v += x; } }
template<typename Container> void INC(Container& C) { ADD(C, 1); }
template<typename Container> void DEC(Container& C) { ADD(C, -1); }
template<typename T> void UNIQUE(T& A) { sort(A.begin(), A.end()); A.erase(unique(A.begin(), A.end()), A.end()); }
template<typename T> auto SUM(const T& v) { if constexpr (ranges::range<T>) { decltype(SUM(*begin(v))) ret = 0; for (const auto& e : v) { ret += SUM(e); } return ret; } else { return v; } }
template<typename Container> ll MAXID(const Container& C) { assert(!C.empty()); return distance(C.begin(), max_element(C.begin(), C.end())); }
template<typename Container> ll MINID(const Container& C) { assert(!C.empty()); return distance(C.begin(), min_element(C.begin(), C.end())); }
template<typename Container, class T> requires ranges::random_access_range<Container> ll FINDID(const Container& C, const T& x) { return distance(C.begin(), find(C.begin(), C.end(), x)); }
template<typename Container, class T> requires ranges::random_access_range<Container> ll LBID(const Container& C, const T& x) { return distance(C.begin(), lower_bound(C.begin(), C.end(), x)); }
template<typename Container, class T> requires ranges::random_access_range<Container> ll UBID(const Container& C, const T& x) { return distance(C.begin(), upper_bound(C.begin(), C.end(), x)); }
template<typename Container> auto RLE(const Container& C) { vector<pair<typename Container::value_type, ll>> ret; for (const auto& x : C) { if (ret.empty() || x != ret.back().first) { ret.emplace_back(x, 1); } else { ret.back().second++; } } return ret; }
template<typename T> vector<T> CUM(const vector<T>& V) { vector<T> ret(V.size()+1, 0); for (ll i = 0; i < (ll)V.size(); i++) { ret[i+1] = ret[i] + V[i]; } return ret; }
template<typename C> bool ISPAL(const C& V) { for (ll i = 0; i < (ll)V.size() / 2; i++) { if (V[i] != V[(ll)V.size() - 1 - i]) return false; } return true; }

// 幾何系マクロ
bool INGRID(ll x, ll y, ll H, ll W) { return x >= 0 && x < H && y >= 0 && y < W; }
template<typename C> vector<C> TRANSPOSE(const vector<C>& G) { if (G.empty()) return {}; ll h = (ll)G.size(), w = (ll)G[0].size(); vector<C> H(w, C(h, typename C::value_type{})); for (ll i = 0; i < w; i++) for (ll j = 0; j < h; j++) H[i][j] = G[j][i]; return H; }
template<typename C> vector<C> ROTATE(const vector<C>& G, ll cnt = 1) { if (G.empty()) return {}; cnt = MOD(cnt, 4); ll h = (ll)G.size(), w = (ll)G[0].size(); vector<C> H(cnt % 2 == 1 ? w : h, C(cnt % 2 == 1 ? h : w, typename C::value_type{})); for (ll i = 0; i < (ll)H.size(); i++) for (ll j = 0; j < (ll)H[0].size(); j++) H[i][j] = G[cnt == 0 ? i : cnt == 1 ? h - 1 - j : cnt == 2 ? h - 1 - i : j][cnt == 0 ? j : cnt == 1 ? i : cnt == 2 ? w - 1 - j : w - 1 - i]; return H; } // 時計回りに90*cnt度回転
ll DIST2(ll x1, ll y1, ll x2, ll y2) { return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); }
ll MDIST(ll x1, ll y1, ll x2, ll y2) { return abs(x1 - x2) + abs(y1 - y2); }

// その他便利マクロ
template<typename T, typename U> bool CHMIN(T& a, const U& b) { if (a > b) { a = b; return true; } return false; }
template<typename T, typename U> bool CHMAX(T& a, const U& b) { if (a < b) { a = b; return true; } return false; }
template<typename... Ts> void SORT(Ts&... xs) { array v = {xs...}; sort(v.begin(), v.end()); ll i = 0; ((xs = v[i++]), ...); }

// 定数
constexpr ll INF = 4'000'000'000'000'000'000LL;
constexpr ll DI[] = {-1, 0, 1, 0};
constexpr ll DJ[] = {0, 1, 0, -1};
constexpr ll DI8[] = {-1, -1, 0, 1, 1, 1, 0, -1};
constexpr ll DJ8[] = {0, 1, 1, 1, 0, -1, -1, -1};
const string DIR = "URDL";

constexpr bool IS_MULTI_CASE = false;


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
