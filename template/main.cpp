#include<bits/stdc++.h>
#include<atcoder/all>
using namespace std;
using namespace atcoder;

using ll = long long;
using mint = modint998244353;

using pll = pair<ll, ll>;
using tll = tuple<ll, ll, ll>;
using tll4 = tuple<ll, ll, ll, ll>;
using vll = vector<ll>;
using vvll =  vector<vll>;
using vdl = vector<double>;
using vvdl = vector<vdl>;
using vch = vector<char>;
using vvch = vector<vch>;
using vst = vector<string>;
using vvst = vector<vst>;
using vbo = vector<bool>;
using vvbo = vector<vbo>;
using vmi = vector<mint>;
using vvmi = vector<mint>;
using vpll = vector<pll>;
using vvpll = vector<vpll>;
using vtll = vector<tll>;
using vvtll = vector<vtll>;
using vtll4 = vector<tll4>;
using vvtll4 = vector<vtll4>;

#define rep(i, n) for(ll i=0;i<(n);i++)
#define rep1(i, n) for(ll i=1;i<=(n);i++)
#define rep3(i, m, n) for(ll i=(m);i<(n);i++)
#define rrep(i, m, n) for(ll i=(m);i>=(n);i--)
#define all(v) (v).begin(),(v).end()
#define rall(v) (v).rbegin(),(v).rend()
#define len(v) (ll)(v).size()

void CIN(){}
template <class Head,class... Tail>
void CIN(Head&& head,Tail&&... tail) {cin>>head; CIN(move(tail)...);}
#define llin(...) ll __VA_ARGS__;CIN(__VA_ARGS__)
#define dlin(...) double __VA_ARGS__;CIN(__VA_ARGS__)
#define chin(...) char __VA_ARGS__;CIN(__VA_ARGS__)
#define stin(...) string __VA_ARGS__;CIN(__VA_ARGS__)
#define cinv(x) for(auto&youso_: (x) )cin>>youso_

const vector<string> YESNO = {"Yes", "No"};
template<class T, class... Ts> void pr(const T& a, const Ts&... b) { cout << a; ((cout << ' ' << b), ...); cout << '\n'; }
template<class... Ts> void prs(const Ts&... a) { ((cout << a << ' '), ...); }
template <typename Container> void prvec(const Container& c) { bool first = true; for (const auto& x : c) {if (!first) {cout << ' ';} cout << x; first = false; } cout << '\n'; }
#define yes pr(YESNO[0])
#define no pr(YESNO[1])
inline bool yn(bool f) { if (f) { yes; return true; } no; return false; }

template<class T> inline bool chmin(T& a, T b){ if(a>b){ a=b; return true; } return false; }
template<class T> inline bool chmax(T& a, T b){ if(a<b){ a=b; return true; } return false; }
template<class T> inline bool inrange(T x, T y, T H, T W) { return x >= 0 && x < H && y >= 0 && y < W; }

const ll INF = 8'000'000'000'000'000'000LL;
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};

const bool multi_case = false;


void solve() {

}


int main() {
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ll t = 1;
    if (multi_case) cin >> t;
    while(t--) {
        solve();
    }
}