#include<bits/stdc++.h>
#include<atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define ll long long
const ll INF = 1'000'000'000'000'000'000LL;

// 組み合わせライブラリ．
// コンストラクタ：Combination<T> cmb;
// Tに型を渡す（modint998244353など．inv(逆元)が定義されている必要あり．）
// fact(n) (条件：n >= 0)
// n!を計算．すでに計算してある場合はO(1)，そうでないなら最大でO(n)．
// finv(n) (条件：n >= 0)
// 1/n!を計算．すでに計算してある場合はO(1)，そうでないなら最大でO(n+log(MOD))；
// binom(n, r) (条件：n >= 0)
// nCrを計算．r < 0やn < rなら0を返す．O(1)もしくは最大でO(n+log(MOD))．
template<class T>
struct Combination {
private:
    vector<T> fact_v, finv_v;

public:
    Combination() {
        fact_v.push_back(1);
        finv_v.push_back(0);
    }

    T fact(ll n) {
        assert(n >= 0);
        while((ll)fact_v.size() <= n) {
            fact_v.push_back(fact_v.back() * fact_v.size());
        }
        return fact_v[n];
    }

    T finv(ll n) {
        assert(n >= 0);
        while((ll)finv_v.size() <= n) {
            finv_v.push_back(0);
        }
        if (finv_v[n] == 0) {
            finv_v[n] = fact(n).inv();
        }
        return finv_v[n];
    }

    T binom(ll n, ll r) {
        assert(n >= 0);
        if (r < 0 || r > n) return 0;
        return fact(n) * finv(r) * finv(n-r);
    }
};


int main() {
    // 使用例：ABC405-E Friut Lineup
    ll A, B, C, D;
    cin >> A >> B >> C >> D;
    
    Combination<mint> cmb;
    mint ans = 0;
    for (ll i = 0; i <= B; i++) {
        ans += cmb.binom(A + i - 1, i) * cmb.binom(B + C + D - i, C);
    }
    cout << ans.val() << endl;
}