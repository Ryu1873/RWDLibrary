#include<bits/stdc++.h>
#include<cassert>
using namespace std;
#define ll long long
const ll INF = 4'000'000'000'000'000'000LL;

// 座標圧縮を行う．
// 計算量はO(NlogN)．
template<typename T>
vector<ll> Compress(vector<T> A) {
    vector<T> A_copy = A;
    sort(A_copy.begin(), A_copy.end());
    A_copy.erase(unique(A_copy.begin(), A_copy.end()), A_copy.end());
    for (ll i = 0; i < (ll)A.size(); i++) {
        A[i] = distance(A_copy.begin(), lower_bound(A_copy.begin(), A_copy.end(), A[i]));
    }
    return A;
}


// 座標圧縮を扱う構造体．
// コンストラクタ
// 1.Compression<T> cmp;
// 要素数0の構造体を構築．
// 2.Compression<T> cmp(v);
// vを要素とする構造体を構築．
// メソッド
// push(x)
// xを追加．
// pos(x)
// xの座標圧縮後の位置を求める．O(logN)．
// get(i)
// i番目に小さい要素を求める．
// size()
// 座標圧縮後のサイズを求める．
// pos,get,sizeのいずれかを呼んだ後にpushすることはできない．
template<typename T>
struct Compression {
private:
    bool calced = false;
    vector<T> v;

    void calc() {
        calced = true;
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());
    }

public:
    Compression(vector<T> v_ = {}) : v(v_) { } 

    void push(T x) {
        assert(!calced);
        v.push_back(x);
    }

    ll pos(T x) {
        if (!calced) calc();
        auto it = lower_bound(v.begin(), v.end(), x);
        assert(it != v.end() && *it == x);
        return distance(v.begin(), it);
    }

    T get(ll i) {
        if (!calced) calc();
        assert(0 <= i && i < (ll)v.size());
        return v[i];
    }

    ll size() {
        if (!calced) calc();
        return v.size();
    }
};


int main() {
    
}