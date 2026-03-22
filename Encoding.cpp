#include<bits/stdc++.h>
using namespace std;
#define ll long long
const ll INF = 1'000'000'000'000'000'000LL;

// string,vectorなどのコンテナを入力として，ランレングス圧縮を行う．
// 返り値は{要素，長さ}を要素とするvector．
template <typename Container>
auto RLE(const Container& c) {
    using T = typename Container::value_type;
    vector<pair<T, ll>> ret;
    for (const auto& x : c) {
        if (ret.empty() || x != ret.back().first) {
            ret.emplace_back(x, 1);
        } else {
            ret.back().second++;
        }
    }
    return ret;
}

int main() {
    
}