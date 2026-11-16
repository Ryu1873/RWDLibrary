#include<bits/stdc++.h>
#include<atcoder/all>
using namespace std;
using namespace atcoder;
#define ll long long
#define rep(i, n) for(ll i=0;i<(n);i++)
#define rep2(i, m, n) for(ll i=(m);i<(n);i++)
#define rrep(i, m, n) for(ll i=(m);i>=(n);i--)
#define all(v) (v).begin(),(v).end()
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
void CIN(){}
template <class Head,class... Tail>
void CIN(Head&& head,Tail&&... tail) {cin>>head; CIN(move(tail)...);}
#define llin(...) ll __VA_ARGS__;CIN(__VA_ARGS__)
#define stin(...) string __VA_ARGS__;CIN(__VA_ARGS__)
#define cinv(x) for(auto&youso_: (x) )cin>>youso_
#define say(x) cout<<(x)<<endl
#define says(x) cout<<(x)<<" "
#define sayv(x) for(auto&youso_: (x) )cout<<youso_<<" ";cout<<endl
#define YES(n) cout << ((n) ? "YES" : "NO") << endl
#define Yes(n) cout << ((n) ? "Yes" : "No") << endl
template<class T> inline bool chmin(T& a, T b){ if(a>b){ a=b; return true; } return false; }
template<class T> inline bool chmax(T& a, T b){ if(a<b){ a=b; return true; } return false; }
const ll INF = 1'000'000'000'000'000'000LL;


// 約数全列挙(O(√N))
vector<ll> divisors(ll N) {
    vector<ll> ret;
    for (ll i = 1; i*i <= N; i++) {
        if (N % i == 0) {
            ret.push_back(i);
            if (i != N / i) {
                ret.push_back(N / i);
            }
        }
    }
    sort(ret.begin(), ret.end());
    return ret;
}


// エラトステネスの篩．N以下の素数判定をO(NloglogN)時間で行う．
vector<bool> isPrimes(ll N) {
    vector<bool> ret(N+1, true);
    ret[0] = false; ret[1] = false;
    for (ll i = 2; i * i <= N; i++) {
        if (ret[i]) {
            for (ll j = i * 2; j <= N; j += i) {
                ret[j] = false;
            }
        }
    }
    return ret;
}


// isPrimesを用いてN以下の素数をO(NloglogN)時間ですべて列挙．
vector<ll> primes(ll N) {
    vector<ll> ret(0);
    vector<bool> isp = isPrimes(N);
    for (ll i = 0; i <= N; i++) {
        if (isp[i]) {
            ret.push_back(i);
        }
    }
    return ret;
}


// N以下の各自然数について「素因数の種類数」をO(NloglogN)時間で求める．
vector<ll> primeFactorVariations(ll N) {
    vector<ll> ret(N+1, 0);
    for(ll i = 2; i <= N; i++) {
        if (ret[i] == 0) {
            for (ll j = i; j <= N; j += i) {
                ret[j]++;
            }
        }
    }
    return ret;
}


// N以下の各自然数について「最小の素因数」をO(NloglogN)時間で求める．
// 素因数分解や約数列挙の前処理として使う．
vector<ll> minPrimeFactors(ll N) {
    vector<ll> ret(N+1, 1);
    for (ll i = 2; i <= N; i++) {
        if (ret[i] == 1) {
            for (ll j = i; j <= N; j += i) {
                if (ret[j] == 1) {
                    ret[j] = i;
                }
            }
        }
    }
    return ret;
}


// 最小の素因数リストmpfを与えることで，nをO(logn)で素因数分解する．
// 返り値は{素因数，指数}のペアを昇順に並べたvector．
vector<pair<ll, ll>> fastPrimeFactorization(ll n, vector<ll>& mpf) {
    vector<pair<ll, ll>> ret(0);
    while (n != 1) {
        ll pf = mpf[n], ex = 0;
        while (n % pf == 0) {
            n /= pf;
            ex++;
        }
        ret.emplace_back(pf, ex);
    }
    return ret;
}


// 最小の素因数リストmpfを与えることで，nの約数をσ(n)をnの約数の個数としてO(σ(n))で列挙する．
// 返り値の順序は未定義．
vector<ll> devisors(ll n, vector<ll>& mpf) {
    vector<ll> ret(1, 1);
    vector<pair<ll, ll>> pf = fastPrimeFactorization(n, mpf);
    for (pair<ll, ll> p : pf) {
        ll sz = ret.size();
        for (ll i = 0; i < sz; i++) {
            ll mul = 1;
            for (ll j = 0; j < p.second; j++) {
                mul *= p.first;
                ret.push_back(ret[i] * mul);
            }
        }
    }
    return ret;
}


int main() {
    
}