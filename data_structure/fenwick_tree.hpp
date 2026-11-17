#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <vector>

/**
 * @brief ACL 互換インターフェースをベースとした拡張 Fenwick Tree (Binary Indexed Tree)
 * @tparam T 要素の型 (デフォルト: long long)
 * @note 0-indexed の長さ N の配列に対し、1点加算・1点更新・1点取得・区間和・
 *       二分探索を O(log N)、初期配列からの構築を O(N) で行う。
 *       二分探索 (lower_bound / upper_bound) を使用する場合は、すべての要素が非負 (a[i] >= 0)
 *       であることを仮定する。
 */
template <typename T = long long>
class FenwickTree {
private:
    long long n_ = 0;
    long long max_pow2_ = 0;
    std::vector<T> data_;

    static long long calc_max_pow2(long long n) noexcept {
        if (n <= 0) return 0;
        return static_cast<long long>(std::bit_floor(static_cast<unsigned long long>(n)));
    }

    void build() {
        max_pow2_ = calc_max_pow2(n_);
        for (long long i = 1; i <= n_; ++i) {
            const long long j = i + (i & -i);
            if (j <= n_) {
                data_[static_cast<std::size_t>(j)] += data_[static_cast<std::size_t>(i)];
            }
        }
    }

    template <bool Upper>
    long long find_prefix_bound(const T& w) const {
        if constexpr (Upper) {
            if (w < T{}) return 0;
        } else {
            if (w <= T{}) return 0;
        }
        long long idx = 0;
        T cur{};
        for (long long step = max_pow2_; step > 0; step >>= 1) {
            if (idx + step <= n_) {
                const T nxt = cur + data_[static_cast<std::size_t>(idx + step)];
                const bool take = Upper ? (nxt <= w) : (nxt < w);
                if (take) {
                    idx += step;
                    cur = nxt;
                }
            }
        }
        return idx + 1;
    }

public:
    using value_type = T;
    using size_type = long long;

    /**
     * @brief 長さ 0 の FenwickTree を構築する
     */
    FenwickTree() = default;

    /**
     * @brief 長さ n、全要素 T{} (0) で FenwickTree を構築する
     * @param n 配列長 (n >= 0)
     * @note 時間計算量: O(N)
     */
    explicit FenwickTree(long long n) { assign(n); }

    /**
     * @brief 長さ n、全要素 val で FenwickTree を構築する
     * @param n 配列長 (n >= 0)
     * @param val 初期値
     * @note 時間計算量: O(N)
     */
    FenwickTree(long long n, const T& val) { assign(n, val); }

    /**
     * @brief 初期配列 v から FenwickTree を構築する
     * @param v 初期配列
     * @note 時間計算量: O(N)
     */
    explicit FenwickTree(const std::vector<T>& v) { assign(v); }

    /**
     * @brief イテレータ範囲 [first, last) から FenwickTree を構築する
     * @tparam InputIt 入力イテレータ型
     * @param first 先頭イテレータ
     * @param last 末尾イテレータ
     * @note 時間計算量: O(N)
     */
    template <std::input_iterator InputIt>
    FenwickTree(InputIt first, InputIt last) {
        assign(first, last);
    }

    /**
     * @brief 配列長を返す
     * @note 時間計算量: O(1)
     */
    long long size() const noexcept { return n_; }

    /**
     * @brief 配列が空 (長さ 0) かどうかを返す
     * @note 時間計算量: O(1)
     */
    bool empty() const noexcept { return n_ == 0; }

    /**
     * @brief 配列を長さ 0 にリセットする
     * @note 時間計算量: O(N)
     */
    void clear() noexcept {
        n_ = 0;
        max_pow2_ = 0;
        data_.clear();
    }

    /**
     * @brief 長さ n、全要素 T{} (0) で再初期化する
     * @param n 配列長 (n >= 0)
     * @note 時間計算量: O(N)
     */
    void assign(long long n) {
        assert(n >= 0);
        n_ = n;
        max_pow2_ = calc_max_pow2(n_);
        if (n_ == 0) {
            data_.clear();
        } else {
            data_.assign(static_cast<std::size_t>(n_ + 1), T{});
        }
    }

    /**
     * @brief 長さ n、全要素 val で再初期化する
     * @param n 配列長 (n >= 0)
     * @param val 初期値
     * @note 時間計算量: O(N)
     */
    void assign(long long n, const T& val) {
        assert(n >= 0);
        n_ = n;
        if (n_ == 0) {
            max_pow2_ = 0;
            data_.clear();
            return;
        }
        data_.assign(static_cast<std::size_t>(n_ + 1), val);
        data_[0] = T{};
        build();
    }

    /**
     * @brief 初期配列 v で再初期化する
     * @param v 初期配列
     * @note 時間計算量: O(N)
     */
    void assign(const std::vector<T>& v) {
        n_ = static_cast<long long>(v.size());
        if (n_ == 0) {
            max_pow2_ = 0;
            data_.clear();
            return;
        }
        data_.resize(static_cast<std::size_t>(n_ + 1));
        data_[0] = T{};
        std::copy(v.begin(), v.end(), data_.begin() + 1);
        build();
    }

    /**
     * @brief イテレータ範囲 [first, last) で再初期化する
     * @tparam InputIt 入力イテレータ型
     * @param first 先頭イテレータ
     * @param last 末尾イテレータ
     * @note 時間計算量: O(N)
     */
    template <std::input_iterator InputIt>
    void assign(InputIt first, InputIt last) {
        if constexpr (std::forward_iterator<InputIt>) {
            n_ = static_cast<long long>(std::distance(first, last));
            if (n_ == 0) {
                max_pow2_ = 0;
                data_.clear();
                return;
            }
            data_.resize(static_cast<std::size_t>(n_ + 1));
            data_[0] = T{};
            std::copy(first, last, data_.begin() + 1);
        } else {
            data_.clear();
            data_.push_back(T{});
            for (; first != last; ++first) {
                data_.push_back(*first);
            }
            n_ = static_cast<long long>(data_.size()) - 1;
            if (n_ == 0) {
                max_pow2_ = 0;
                data_.clear();
                return;
            }
        }
        build();
    }

    /**
     * @brief p 番目の要素に x を加算する (a[p] += x)
     * @param p インデックス (0 <= p < n)
     * @param x 加算する値
     * @note 時間計算量: O(log N)
     */
    void add(long long p, const T& x) {
        assert(0 <= p && p < n_);
        for (long long i = p + 1; i <= n_; i += i & -i) {
            data_[static_cast<std::size_t>(i)] += x;
        }
    }

    /**
     * @brief p 番目の要素を x に更新する (a[p] = x)
     * @param p インデックス (0 <= p < n)
     * @param x 新しい値
     * @note 時間計算量: O(log N)
     */
    void set(long long p, const T& x) {
        assert(0 <= p && p < n_);
        add(p, x - get(p));
    }

    /**
     * @brief p 番目の要素 a[p] を取得する
     * @param p インデックス (0 <= p < n)
     * @return a[p] の値
     * @note 時間計算量: 最悪 O(log N) / 一様アクセス時平均 O(1)
     */
    T get(long long p) const {
        assert(0 <= p && p < n_);
        T res = data_[static_cast<std::size_t>(p + 1)];
        const long long lca = p & (p + 1);
        for (long long i = p; i != lca; i -= i & -i) {
            res -= data_[static_cast<std::size_t>(i)];
        }
        return res;
    }

    /**
     * @brief p 番目の要素 a[p] を取得する (get(p) と等価)
     * @param p インデックス (0 <= p < n)
     * @note 時間計算量: 最悪 O(log N) / 一様アクセス時平均 O(1)
     */
    T operator[](long long p) const { return get(p); }

    /**
     * @brief 区間 [0, r) の和を返す
     * @param r 右端 (0 <= r <= n)
     * @return a[0] + ... + a[r - 1]
     * @note 時間計算量: O(log N)
     */
    T sum(long long r) const {
        assert(0 <= r && r <= n_);
        T res{};
        for (long long i = r; i > 0; i -= i & -i) {
            res += data_[static_cast<std::size_t>(i)];
        }
        return res;
    }

    /**
     * @brief 区間 [l, r) の和を返す
     * @param l 左端 (0 <= l <= r)
     * @param r 右端 (l <= r <= n)
     * @return a[l] + ... + a[r - 1]
     * @note 時間計算量: O(log N)
     */
    T sum(long long l, long long r) const {
        assert(0 <= l && l <= r && r <= n_);
        return sum(r) - sum(l);
    }

    /**
     * @brief sum(0, r) >= w を満たす最小の r (0 <= r <= n) を返す
     * @param w 目標累積和
     * @return 条件を満たす最小の r (存在しない場合は n + 1)
     * @note すべての要素が非負 (a[i] >= 0) であることを仮定する。時間計算量: O(log N)
     */
    long long lower_bound(const T& w) const {
        return find_prefix_bound<false>(w);
    }

    /**
     * @brief sum(0, r) > w を満たす最小の r (0 <= r <= n) を返す
     * @param w 目標累積和
     * @return 条件を満たす最小の r (存在しない場合は n + 1)
     * @note すべての要素が非負 (a[i] >= 0) であることを仮定する。時間計算量: O(log N)
     */
    long long upper_bound(const T& w) const {
        return find_prefix_bound<true>(w);
    }
};

FenwickTree(long long) -> FenwickTree<long long>;

template <typename T>
FenwickTree(long long, const T&) -> FenwickTree<T>;

template <typename T>
FenwickTree(const std::vector<T>&) -> FenwickTree<T>;

template <std::input_iterator InputIt>
FenwickTree(InputIt, InputIt)
    -> FenwickTree<typename std::iterator_traits<InputIt>::value_type>;
