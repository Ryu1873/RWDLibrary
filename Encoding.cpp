#include<bits/stdc++.h>
using namespace std;
#define ll long long
const ll INF = 1'000'000'000'000'000'000LL;

// 座標圧縮を行う．
// 計算量はO(NlogN)．
template<typename T>
vector<ll> Compression(vector<T> A) {
    vector<T> A_copy = A;
    sort(A_copy.begin(), A_copy.end());
    A_copy.erase(unique(A_copy.begin(), A_copy.end()), A_copy.end());
    for (ll i = 0; i < (ll)A.size(); i++) {
        A[i] = distance(A_copy.begin(), lower_bound(A_copy.begin(), A_copy.end(), A[i]));
    }
    return A;
}


int main() {
    vector<ll> A = {1, 8, 5, 5 ,3, 1};
    A = Compression(A);
    for (ll i = 0; i < 6; i++) cout << A[i] << endl;
}