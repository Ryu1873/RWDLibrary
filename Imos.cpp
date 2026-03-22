#include<bits/stdc++.h>
using namespace std;
#define ll long long
const ll INF = 1'000'000'000'000'000'000LL;


// 基本のいもす法．区間は半開区間[l,r)．返り値はN+1要素のvector．
vector<ll> imos(ll N, vector<ll>& L, vector<ll>& R) {
    vector<ll> ret(N+1, 0);
    ll M = L.size();
    for (ll i = 0; i < M; i++) {
        ret[L[i]]++;
        ret[R[i]]--;
    }
    for(ll i = 1; i <= N; i++) {
        ret[i] += ret[i-1];
    }
    return ret;
}

// 2次元いもす法．返り値は(H+1)x(W+1)要素のvector．
vector<vector<ll>> imos2d(ll H, ll W, vector<ll>& LX, vector<ll>& RX, vector<ll>& UY, vector<ll>& DY) {
    vector<vector<ll>> ret(H+1, vector<ll>(W+1, 0));
    ll N = LX.size();
    for (ll i = 0; i < N; i++) {
        ret[UY[i]][LX[i]]++;
        ret[UY[i]][RX[i]]--;
        ret[DY[i]][LX[i]]--;
        ret[DY[i]][RX[i]]++;
    }
    for (ll i = 0; i <= H; i++) {
        for (ll j = 1; j <= W; j++) {
            ret[i][j] += ret[i][j-1];
        }
    }
    for (ll j = 0; j <= W; j++) {
        for (ll i = 1; i <= H; i++) {
            ret[i][j] += ret[i-1][j];
        }
    }
    return ret;
}


int main() {
    
}