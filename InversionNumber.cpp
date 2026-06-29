#include<bits/stdc++.h>
#include <atcoder/fenwicktree>
using namespace std;
#define ll long long
const ll INF = 1'000'000'000'000'000'000LL;

// Aの転倒数を求める．
// 計算量はO(NlogN)．
template<typename T>
ll inversionNumber(vector<T>& A) {
    ll N = A.size(), ret = 0;
    vector<T> A_copy = A;
    sort(A_copy.begin(), A_copy.end());
    A_copy.erase(unique(A_copy.begin(), A_copy.end()), A_copy.end());
    atcoder::fenwick_tree<ll> bit(A_copy.size());
    for (ll i = 0; i < N; i++) {
        ll id = distance(A_copy.begin(), lower_bound(A_copy.begin(), A_copy.end(), A[i]));
        ret += bit.sum(id + 1, N);
        bit.add(id, 1);
    }
    return ret;
}


int main() {

}