#include<bits/stdc++.h>
using namespace std;
#define ll long long
const ll INF = 1'000'000'000'000'000'000LL;


struct RollingHash {
    static const ll mod = (1ll << 61) - 1;
    ll base;
    vector<ll> hash, power;

    // mod 2^61-1 での計算を行う
    static inline ll calc_mod(__uint128_t val) {
        val = (val & mod) + (val >> 61);
        if (val >= mod) val -= mod;
        return (ll)val;
    }

    // 乗算
    static inline ll mul(ll a, ll b) {
        return calc_mod((__uint128_t)a * b);
    }

    // コンストラクタ：文字列と基数を受け取り、前計算を行う O(N)
    RollingHash(const string &s, ll base) : base(base) {
        ll n = s.size();
        hash.assign(n + 1, 0);
        power.assign(n + 1, 1);
        for (ll i = 0; i < n; i++) {
            hash[i + 1] = calc_mod((__uint128_t)hash[i] * base + s[i]);
            power[i + 1] = mul(power[i], base);
        }
    }

    // [l, r) の区間のハッシュ値を取得 (0-indexed) O(1)
    ll get(ll l, ll r) const {
        ll res = hash[r] + mod - mul(hash[l], power[r - l]);
        if (res >= mod) res -= mod;
        return res;
    }

    // 文字列全体のハッシュ値を取得 O(1)
    ll get_all() const {
        return get(0, hash.size() - 1);
    }
    
    // 2つのハッシュ値を結合する（長さ len2 の文字列のハッシュ h2 を、h1 の後ろに繋げる）
    ll concat(ll h1, ll h2, ll len2) const {
        return calc_mod((__uint128_t)h1 * power[len2] + h2);
    }
};

// 実行時にランダムな基数を生成する関数（ハッシュ衝突対策）
ll generate_base() {
    mt19937_64 mt(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<ll> rand(256, RollingHash::mod - 1);
    return rand(mt);
}


int main() {
    
    // 使用例(ABC430-E)
    ll base = generate_base();
    ll T; cin >> T;
    while (T--) {
        string A, B;
        cin >> A >> B;
        ll N = A.size();
        string A2 = A + A;
        RollingHash RA(A2, base), RB(B, base);
        ll ans = -1;
        for (ll i = 0; i < N; i++) {
            if (RA.get(i, i+N) == RB.get_all()) {
                ans = i;
                break;
            }
        }
        cout << ans << '\n';
    }
}