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


int main() {
    vector<ll> A = {1, 5, 3, 6};
    vector<ll> B = supersetZeta(A);
    for(ll i = 0; i < 4; i++) cout << B[i] << " ";
    cout << endl;
    vector<ll> C = supersetMobius(B);
    for(ll i = 0; i < 4; i++) cout << C[i] << " ";
    cout << endl;
}