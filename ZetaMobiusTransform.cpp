#include<bits/stdc++.h>
using namespace std;
#define ll long long
const ll INF = 1'000'000'000'000'000'000LL;


// 部分集合に対する高速ゼータ変換．
// Aは２冪である必要がある．
// Aの要素数を2^nとして，計算量はO(n・2^n)．
template<typename T>
vector<T> subsetZeta(vector<T> A) {
    ll N = 1;
    while((1ll << N) < A.size()) N++;
    assert((1ll << N) == A.size());
    for (ll i = 0; i < N; i++) {
        ll b = 1ll << i;
        for (ll l = 0, r = b; l < (1ll << N); l += 2 * b, r += 2 * b) {
            for (ll t = l; t < r; t++) A[t + b] += A[t];
        }
    }
    return A;
}


// 部分集合に対する高速メビウス変換．
// Aは２冪である必要がある．
// Aの要素数を2^nとして，計算量はO(n・2^n)．
template<typename T>
vector<T> subsetMobius(vector<T> A) {
    ll N = 1;
    while((1ll << N) < A.size()) N++;
    assert((1ll << N) == A.size());
    for (ll i = 0; i < N; i++) {
        ll b = 1ll << i;
        for (ll l = 0, r = b; l < (1ll << N); l += 2 * b, r += 2 * b) {
            for (ll t = l; t < r; t++) A[t + b] -= A[t];
        }
    }
    return A;
}


// 上位集合に対する高速ゼータ変換．
// Aは２冪である必要がある．
// Aの要素数を2^nとして，計算量はO(n・2^n)．
template<typename T>
vector<T> supersetZeta(vector<T> A) {
    ll N = 1;
    while((1ll << N) < A.size()) N++;
    assert((1ll << N) == A.size());
    for (ll i = 0; i < N; i++) {
        ll b = 1ll << i;
        for (ll l = 0, r = b; l < (1ll << N); l += 2 * b, r += 2 * b) {
            for (ll t = l; t < r; t++) A[t] += A[t + b];
        }
    }
    return A;
}


// 上位集合に対する高速メビウス変換．
// Aは２冪である必要がある．
// Aの要素数を2^nとして，計算量はO(n・2^n)．
template<typename T>
vector<T> supersetMobius(vector<T> A) {
    ll N = 1;
    while((1ll << N) < A.size()) N++;
    assert((1ll << N) == A.size());
    for (ll i = 0; i < N; i++) {
        ll b = 1ll << i;
        for (ll l = 0, r = b; l < (1ll << N); l += 2 * b, r += 2 * b) {
            for (ll t = l; t < r; t++) A[t] -= A[t + b];
        }
    }
    return A;
}


// 多次元累積和及びいもす法．
// コンストラクタ：MultidimentionalCumulativeSum<T> S(len); lenには各次元の長さを入れる．初期値は全て0．
// add(p, x):座標pにxを足す．
// range_add(l, r, x):いもす法用．(l,rは半開区間)(未完成)
// calc():累積和の計算を行う．O(DN)くらい．
// get(p):いもす法用．[0, p]の値を取得．
// sum(l, r):[l,r)の和を取得．O(D2^D)くらい．
template<typename T> 
struct MultidimensionalCumulativeSum {
private:
    ll D;
    vector<ll> len;
    vector<T> dat;

    // 座標を整数にエンコードする．はみ出ている場合は-1を返す．
    ll encode(vector<ll>& p) {
        assert(p.size() == D);
        ll ret = 0;
        for (ll i = 0; i < D; i++) {
            if (p[i] < 0 || len[i] <= p[i]) {
                ret = -1;
                break;
            }
            ret += p[i];
            if (i < D - 1) ret *= len[i];
        }
        return ret;
    }

public:
    MultidimensionalCumulativeSum(vector<ll>& len_) : D(len_.size()), len(len_) { 
        ll tmp = 1;
        for (ll i = 0; i < D; i++) {
            tmp *= len[i];
        }
        dat.assign(tmp, 0);
    }

    void add(vector<ll>& p, T x) {
        ll tmp = encode(p);
        if (tmp != -1) dat[tmp] += x;
    }

    void range_add(vector<ll>& l, vector<ll>& r, T x) {
        add(l, x);
        add(r, -x);
    }

    void calc() {
        ll tmp = 1;
        for (ll d = 0; d < D; d++) {
            for (ll i = 0; i < (ll)dat.size(); i++) {
                if ((i / tmp) % len[d] == 0) continue;
                dat[i] += dat[i - tmp];
            }
            tmp *= len[d];
        }
    }

    T get(vector<ll>& p) {
        ll tmp = encode(p);
        return tmp == -1 ? 0 : dat[tmp];
    }

    T sum(vector<ll>& l, vector<ll>& r) {
        assert(l.size() == D && r.size() == D);
        T ret = 0;
        for (ll b = 0; b < (1 << D); b++) {
            vector<ll> v(D);
            for (ll i = 0; i < D; i++) {
                if (b >> i & 1) {
                    v[i] = r[i] - 1;
                } else {
                    v[i] = l[i] - 1;
                }
            }
            if (__builtin_popcount(b) % 2 == D % 2) {
                ret += get(v);
            } else {
                ret -= get(v);
            }
        }
        return ret;
    }
};

int main() {
    
}