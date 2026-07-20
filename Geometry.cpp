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

// 直線を扱う構造体．
// コンストラクタ：Line line(x1, y1, x2, y2):2点を通る直線．
//              Line line(a, b, c)；直接係数を代入．
//順序：傾き→y切片→x切片の順に比較．y軸に平行な直線は傾き無限大とする．
//is_parallel(other)：2直線が平行かどうか判定．
//is_perpendicular(other)：2直線が直交しているかどうか判定．
struct Line {
private:
    ll a, b, c;

    void normalize() {
        ll g = gcd(abs(a), gcd(abs(b), abs(c)));
        a /= g, b /= g, c /= g;
        if (a < 0 || (a == 0 && b < 0)) {
            a *= -1, b *= -1, c *= -1;
        }
    }

public:
    Line(ll a_, ll b_, ll c_) : a(a_), b(b_), c(c_) {
        normalize();
    }
    Line(ll x1, ll y1, ll x2, ll y2) {
        assert(x1 != x2 || y1 != y2);
        a = y1 - y2;
        b = x2 - x1;
        c = -a * x1 - b * y1;
        normalize();
    }

    bool operator == (Line& other) { return a == other.a && b == other.b && c == other.c; }
    bool operator < (Line& other) {
        if (b == 0 || other.b == 0) {
            return (b == 0 && other.b == 0) ? (c * other.a > other.c * a) : (b != 0);
        } else if (a * other.b == other.a * b) {
            return c * other.b > other.c * b;
        } else {
            return a * other.b > other.a * b;
        }
    }
    bool operator > (Line& other) { return other > *this; }
    bool operator <= (Line& other) { return !(other < *this); }
    bool operator >= (Line& other) { return !(*this < other); }
    bool operator != (Line& other) { return !(*this == other); }

    bool is_parallel(Line& other) {
        return a * other.b == other.a * b;
    }

    bool is_perpendicular(Line& other) {
        return a * other.a + b * other.b == 0;
    }
};


// 2点の垂直二等分線を求める．
Line perpendicular_bisector(ll x1, ll y1, ll x2, ll y2) {
    ll a = 2 * (x2 - x1);
    ll b = 2 * (y2 - y1);
    ll c = -(x2 * x2 + y2 * y2 - x1 * x1 - y1 * y1);
    return Line(a, b, c);
}


// 偏角を順序関係とする構造体．原点(x=0,y=0)の動作は未定義．
struct AnglePoint {
    ll x;
    ll y;

    // 半平面による分類．
    // upper:[0°, 180°) lower:[180°, 360°)．
    bool is_upper() const {
        return (y > 0 || (y == 0 && x > 0));
    }

    // 外積．
    ll cross_product(const AnglePoint& other) const {
        return x * other.y - other.x * y;
    }

    // 順序関係 (<)
    bool operator < (const AnglePoint& other) const {
        if (this->is_upper() != other.is_upper()) return this->is_upper(); // 異なる半平面にある場合，上にある方が小さい．
        return cross_product(other) > 0;                                   // 同じ半平面にある場合，外積で判別．
    }

    bool operator > (const AnglePoint& other) const { return other < *this; }
    bool operator == (const AnglePoint& other) const { return !(other < *this) && !(*this < other); }
    bool operator <= (const AnglePoint& other) const { return !(other < *this); }
    bool operator >= (const AnglePoint& other) const { return !(*this < other); }
    bool operator != (const AnglePoint& other) const { return !(*this == other); }
};


int main() {
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
}