#include<bits/stdc++.h>
#include<atcoder/all>
using namespace std;
using namespace atcoder;
#define ll long long
const ll INF = 1'000'000'000'000'000'000LL;


// LISを1つ求める．
vector<ll> LIS(vector<ll>& v) {
    ll N = v.size();
    vector<ll> dp, pos(N);
    for (ll i = 0; i < N; i++) {
        ll id = distance(dp.begin(), lower_bound(dp.begin(), dp.end(), v[i]));
        if (id == (ll)dp.size()) {
            dp.push_back(v[i]);
        } else {
            dp[id] = v[i];
        }
    }
    ll j = (ll)dp.size() - 1;
    for (ll i = N-1; i >= 0; i--) {
        if (pos[i] == j) {
            dp[j] = v[i];
            j--;
        }
    }
    return dp;
}


int main() {
    ll N; cin >> N;
    vector<ll> A(N);
    for (ll i = 0; i < N; i++) {
        cin >> A[i];
    }
    vector<ll> lis = LIS(A);
    for (ll i = 0; i < (ll)lis.size(); i++) {
        cout << lis[i] << " ";
    }
    cout << endl;
}
