#include<bits/stdc++.h>
using namespace std;
#define ll long long
const ll INF = 1'000'000'000'000'000'000LL;

// 不定方程式ax+by=cの解を一つ求める．
// 解が無い場合は{INF, INF}を返す．
template<typename T>
pair<T, T> solveBezout(T a, T b, T c) {
    if (a == 0 && b == 0) {
        if (c == 0) {
            return {0, 0};
        } else {
            return {INF, INF};
        }
    }

    // ax+by=gcd(|a|,|b|)を満たすx,yを求める．
    auto extgcd = [](this auto&& extgcd, T a, T b) -> tuple<T, T, T> {
        T x, y, g;
        if (b == 0) {
            x = (a >= 0 ? 1 : -1);
            y = 0;
            g = abs(a);
        } else {
            T x1, y1;
            tie(x1, y1, g) = extgcd(b, a % b);
            x = y1;
            y = x1 - (a / b) * y1;
        }

        return {x, y, g};
    };

    auto [x, y, g] = extgcd(a, b);
    if (c % g != 0) {
        return {INF, INF};
    }
    T k = c / g;
    return {x*k, y*k};
}


int main() {
    
}