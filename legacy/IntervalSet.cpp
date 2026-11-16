#include<bits/stdc++.h>
using namespace std;
#define ll long long
const ll INF = 1'000'000'000'000'000'000LL;

template<typename T>
class IntervalSet {
private:
    T sum_length; // 区間の合計の長さを保持する変数

public:
    std::set<std::pair<T, T>> st;

    IntervalSet() : sum_length(0) {}

    // 区間 [l, r) を追加する
    void insert(T l, T r) {
        if (l >= r) return;
        
        auto it = st.upper_bound({l, std::numeric_limits<T>::max()});
        if (it != st.begin()) {
            it--;
        }
        if (it != st.end() && it->second < l) {
            it++;
        }
        
        // 被っている、または隣接している区間を削除し、合計長さから引く
        while (it != st.end() && it->first <= r) {
            l = std::min(l, it->first);
            r = std::max(r, it->second);
            
            sum_length -= (it->second - it->first); // 削除する区間の長さを引く
            it = st.erase(it);
        }
        
        // マージされた新しい区間を追加し、合計長さに足す
        st.insert({l, r});
        sum_length += (r - l); 
    }

    // 区間 [l, r) を削除する
    void erase(T l, T r) {
        if (l >= r) return;
        
        auto it = st.upper_bound({l, std::numeric_limits<T>::max()});
        if (it != st.begin()) {
            it--;
        }
        if (it != st.end() && it->second <= l) {
            it++;
        }
        
        // 削除区間に被っている既存の区間を処理する
        while (it != st.end() && it->first < r) {
            T nl = it->first;
            T nr = it->second;
            
            sum_length -= (nr - nl); // 一旦、既存の区間の長さを引く
            it = st.erase(it);
            
            // 左側が余る場合は再挿入し、長さを足す
            if (nl < l) {
                st.insert({nl, l});
                sum_length += (l - nl);
            }
            // 右側が余る場合は再挿入し、長さを足す
            if (r < nr) {
                st.insert({r, nr});
                sum_length += (nr - r);
            }
        }
    }

    // 現在の区間の合計の長さを O(1) で返す
    T get_total_length() const {
        return sum_length;
    }

    // x を含む区間 [l, r) を返す
    std::pair<T, T> get(T x) const {
        auto it = st.upper_bound({x, std::numeric_limits<T>::max()});
        if (it == st.begin()) return {x, x};
        it--;
        if (it->first <= x && x < it->second) {
            return *it;
        }
        return {x, x};
    }

    // x と y が同じ区間に含まれるか判定する
    bool same(T x, T y) const {
        auto ix = get(x);
        auto iy = get(y);
        return ix.first != ix.second && ix == iy;
    }

    // 現在の区間情報を出力（デバック用）
    void print() const {
        for (const auto& p : st) {
            std::cout << "[" << p.first << ", " << p.second << ") ";
        }
        std::cout << " (Total Length: " << sum_length << ")" << std::endl;
    }
};


int main() {
    // 使用例（ABC435-E「Cover query」）

    ll N, Q; cin >> N >> Q;
    
    IntervalSet<ll> iset;
    while(Q--) {
        ll l, r; cin >> l >> r; l--;
        iset.insert(l, r);
        cout << N - iset.get_total_length() << "\n";
    }
}