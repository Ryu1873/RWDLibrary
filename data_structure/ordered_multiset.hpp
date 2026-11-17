#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

/**
 * @brief 重複あり順序付き集合 (AVL木による平衡二分探索木)
 * @tparam T 要素の型 (デフォルト: long long)
 * @tparam Compare 狭義弱順序を表す比較関数型 (デフォルト: std::less<T>)
 * @note 各ノードで同値要素の個数を管理し、挿入・削除・検索・k番目取得・区間個数取得を
 *       すべて最悪時間計算量 O(log N) で処理する。
 */
template <typename T = long long, typename Compare = std::less<T>>
class OrderedMultiSet {
private:
    struct Node {
        T key;
        long long left = -1;
        long long right = -1;
        long long cnt = 1;
        long long size = 1;
        long long height = 1;
    };

public:
    using value_type = T;
    using key_compare = Compare;

    /**
     * @brief 空の OrderedMultiSet を構築する
     */
    OrderedMultiSet() = default;

    /**
     * @brief 指定した比較関数で空の OrderedMultiSet を構築する
     * @param comp 比較関数
     */
    explicit OrderedMultiSet(Compare comp) : comp_(std::move(comp)) {}

    /**
     * @brief 初期化リストから OrderedMultiSet を構築する
     * @param init 初期要素列 (重複を含む場合すべて格納される)
     * @param comp 比較関数
     * @note 時間計算量: O(|init| log |init|)
     */
    OrderedMultiSet(std::initializer_list<T> init, Compare comp = Compare())
        : comp_(std::move(comp)) {
        reserve(static_cast<long long>(init.size()));
        for (const auto& x : init) {
            insert(x);
        }
    }

    /**
     * @brief イテレータ範囲 [first, last) から OrderedMultiSet を構築する
     * @tparam InputIt 入力イテレータ型
     * @param first 先頭イテレータ
     * @param last 末尾イテレータ
     * @param comp 比較関数
     */
    template <typename InputIt>
    OrderedMultiSet(InputIt first, InputIt last, Compare comp = Compare())
        : comp_(std::move(comp)) {
        for (; first != last; ++first) {
            insert(*first);
        }
    }

    /**
     * @brief 格納されている総要素数 (重複を含む) を返す
     * @note 時間計算量: O(1)
     */
    long long size() const noexcept { return node_size(root_); }

    /**
     * @brief 集合が空かどうかを返す
     * @note 時間計算量: O(1)
     */
    bool empty() const noexcept { return root_ == -1; }

    /**
     * @brief すべての要素を削除して空にする
     * @note 時間計算量: O(N)
     */
    void clear() noexcept {
        nodes_.clear();
        free_list_.clear();
        root_ = -1;
    }

    /**
     * @brief 内部ノードプールのメモリを事前確保する
     * @param n 確保するノード数 (n >= 0)
     */
    void reserve(long long n) {
        assert(n >= 0);
        nodes_.reserve(static_cast<std::size_t>(n));
    }

    /**
     * @brief 要素 x を 1 つ挿入する (重複を許容する)
     * @param x 挿入する値
     * @note 時間計算量: O(log N)
     */
    void insert(const T& x) {
        bool height_changed = false;
        root_ = insert_impl(root_, x, height_changed);
    }

    /**
     * @brief 要素 x をムーブして 1 つ挿入する (重複を許容する)
     * @param x 挿入する値
     * @note 時間計算量: O(log N)
     */
    void insert(T&& x) {
        bool height_changed = false;
        root_ = insert_impl(root_, std::move(x), height_changed);
    }

    /**
     * @brief 引数から要素を直接構築して 1 つ挿入する
     * @note 時間計算量: O(log N)
     */
    template <typename... Args>
    void emplace(Args&&... args) {
        T val(std::forward<Args>(args)...);
        insert(std::move(val));
    }

    /**
     * @brief 要素 x を 1 つだけ削除する (存在しない場合は何もしない)
     * @param x 削除する値
     * @return 削除された場合 true、存在しなかった場合 false
     * @note 時間計算量: O(log N)
     */
    bool erase(const T& x) {
        bool erased = false;
        bool node_removed = false;
        root_ = erase_impl(root_, x, erased, node_removed);
        return erased;
    }

    /**
     * @brief 要素 x をすべて削除する
     * @param x 削除する値
     * @return 削除した個数
     * @note 時間計算量: O(log N)
     */
    long long erase_all(const T& x) {
        long long removed_cnt = 0;
        root_ = erase_all_impl(root_, x, removed_cnt);
        return removed_cnt;
    }

    /**
     * @brief 0-indexed で k 番目の要素を 1 つだけ削除して返す
     * @param k 削除する要素の順位 (0-indexed)
     * @return k が範囲内なら削除された要素、範囲外なら std::nullopt
     * @note 時間計算量: O(log N)
     */
    std::optional<T> erase_kth(long long k) {
        if (k < 0 || k >= size()) return std::nullopt;
        std::optional<T> removed;
        bool node_removed = false;
        root_ = erase_kth_impl(root_, k, removed, node_removed);
        return removed;
    }

    /**
     * @brief 要素 x が集合に 1 つ以上含まれるかを判定する
     * @param x 検索する値
     * @note 時間計算量: O(log N)
     */
    bool contains(const T& x) const { return count(x) > 0; }

    /**
     * @brief 要素 x の個数を返す
     * @param x 検索する値
     * @note 時間計算量: O(log N)
     */
    long long count(const T& x) const {
        long long cur = root_;
        while (cur != -1) {
            if (comp_(x, nodes_[cur].key)) {
                cur = nodes_[cur].left;
            } else if (comp_(nodes_[cur].key, x)) {
                cur = nodes_[cur].right;
            } else {
                return nodes_[cur].cnt;
            }
        }
        return 0;
    }

    /**
     * @brief 0-indexed で k 番目に小さい要素を返す
     * @param k 順位 (0-indexed)
     * @return 0 <= k < size() なら該当要素、範囲外なら std::nullopt
     * @note 時間計算量: O(log N)
     */
    std::optional<T> kth(long long k) const {
        const T* ptr = find_kth_ptr(k);
        if (ptr == nullptr) return std::nullopt;
        return *ptr;
    }

    /**
     * @brief 0-indexed で k 番目に小さい要素への参照を返す (範囲外は assert)
     * @param k 順位 (0 <= k < size())
     * @note 時間計算量: O(log N)
     */
    const T& operator[](long long k) const {
        assert(0 <= k && k < size());
        return *find_kth_ptr(k);
    }

    /**
     * @brief 最小の要素を返す
     * @return 空でなければ最小要素、空なら std::nullopt
     * @note 時間計算量: O(log N)
     */
    std::optional<T> min() const { return kth(0); }

    /**
     * @brief 最大の要素を返す
     * @return 空でなければ最大要素、空なら std::nullopt
     * @note 時間計算量: O(log N)
     */
    std::optional<T> max() const { return kth(size() - 1); }

    /**
     * @brief x 以上の最小の要素を返す (comp_ 順で !comp_(e, x) となる最小の e)
     * @param x 基準値
     * @return 該当要素、存在しない場合は std::nullopt
     * @note 時間計算量: O(log N)
     */
    std::optional<T> lower_bound(const T& x) const {
        long long cur = root_;
        long long ans = -1;
        while (cur != -1) {
            if (!comp_(nodes_[cur].key, x)) {
                ans = cur;
                cur = nodes_[cur].left;
            } else {
                cur = nodes_[cur].right;
            }
        }
        if (ans == -1) return std::nullopt;
        return nodes_[ans].key;
    }

    /**
     * @brief x より大きい最小の要素を返す (comp_ 順で comp_(x, e) となる最小の e)
     * @param x 基準値
     * @return 該当要素、存在しない場合は std::nullopt
     * @note 時間計算量: O(log N)
     */
    std::optional<T> upper_bound(const T& x) const {
        long long cur = root_;
        long long ans = -1;
        while (cur != -1) {
            if (comp_(x, nodes_[cur].key)) {
                ans = cur;
                cur = nodes_[cur].left;
            } else {
                cur = nodes_[cur].right;
            }
        }
        if (ans == -1) return std::nullopt;
        return nodes_[ans].key;
    }

    /**
     * @brief x 未満の最大の要素を返す (comp_ 順で comp_(e, x) となる最大の e)
     * @param x 基準値
     * @return 該当要素、存在しない場合は std::nullopt
     * @note 時間計算量: O(log N)
     */
    std::optional<T> prev(const T& x) const {
        long long cur = root_;
        long long ans = -1;
        while (cur != -1) {
            if (comp_(nodes_[cur].key, x)) {
                ans = cur;
                cur = nodes_[cur].right;
            } else {
                cur = nodes_[cur].left;
            }
        }
        if (ans == -1) return std::nullopt;
        return nodes_[ans].key;
    }

    /**
     * @brief x 以下の最大の要素を返す (comp_ 順で !comp_(x, e) となる最大の e)
     * @param x 基準値
     * @return 該当要素、存在しない場合は std::nullopt
     * @note 時間計算量: O(log N)
     */
    std::optional<T> prev_or_equal(const T& x) const {
        long long cur = root_;
        long long ans = -1;
        while (cur != -1) {
            if (!comp_(x, nodes_[cur].key)) {
                ans = cur;
                cur = nodes_[cur].right;
            } else {
                cur = nodes_[cur].left;
            }
        }
        if (ans == -1) return std::nullopt;
        return nodes_[ans].key;
    }

    /**
     * @brief x 未満の要素数 (重複を含む) を返す (comp_(e, x) を満たす要素 e の個数)
     * @param x 基準値
     * @note 時間計算量: O(log N)
     */
    long long count_less(const T& x) const {
        long long cur = root_;
        long long cnt = 0;
        while (cur != -1) {
            if (comp_(nodes_[cur].key, x)) {
                cnt += nodes_[cur].cnt + node_size(nodes_[cur].left);
                cur = nodes_[cur].right;
            } else {
                cur = nodes_[cur].left;
            }
        }
        return cnt;
    }

    /**
     * @brief x 以下の要素数 (重複を含む) を返す (!comp_(x, e) を満たす要素 e の個数)
     * @param x 基準値
     * @note 時間計算量: O(log N)
     */
    long long count_less_equal(const T& x) const {
        long long cur = root_;
        long long cnt = 0;
        while (cur != -1) {
            if (!comp_(x, nodes_[cur].key)) {
                cnt += nodes_[cur].cnt + node_size(nodes_[cur].left);
                cur = nodes_[cur].right;
            } else {
                cur = nodes_[cur].left;
            }
        }
        return cnt;
    }

    /**
     * @brief x 以上の要素数 (重複を含む) を返す (!comp_(e, x) を満たす要素 e の個数)
     * @param x 基準値
     * @note 時間計算量: O(log N)
     */
    long long count_greater_equal(const T& x) const {
        return size() - count_less(x);
    }

    /**
     * @brief x より大きい要素数 (重複を含む) を返す (comp_(x, e) を満たす要素 e の個数)
     * @param x 基準値
     * @note 時間計算量: O(log N)
     */
    long long count_greater(const T& x) const {
        return size() - count_less_equal(x);
    }

    /**
     * @brief L 以上 R 未満の要素数 (重複を含む) を返す (半開区間 [l, r))
     * @param l 下限 (含む)
     * @param r 上限 (含まない)
     * @return !comp_(e, l) && comp_(e, r) を満たす要素 e の個数 (l >= r の場合は 0)
     * @note 時間計算量: O(log N)
     */
    long long count_range(const T& l, const T& r) const {
        if (!comp_(l, r)) return 0;
        return count_less(r) - count_less(l);
    }

    /**
     * @brief 全要素 (重複を含む) を昇順 (comp_ 順) の std::vector として返す
     * @note 時間計算量: O(N)
     */
    std::vector<T> to_vector() const {
        std::vector<T> res;
        res.reserve(static_cast<std::size_t>(size()));
        inorder(root_, res);
        return res;
    }

private:
    std::vector<Node> nodes_;
    std::vector<long long> free_list_;
    long long root_ = -1;
    [[no_unique_address]] Compare comp_{};

    long long node_size(long long t) const noexcept {
        return t == -1 ? 0 : nodes_[t].size;
    }

    long long node_height(long long t) const noexcept {
        return t == -1 ? 0 : nodes_[t].height;
    }

    void pull(long long t) noexcept {
        nodes_[t].size =
            nodes_[t].cnt + node_size(nodes_[t].left) + node_size(nodes_[t].right);
        nodes_[t].height =
            1 + std::max(node_height(nodes_[t].left), node_height(nodes_[t].right));
    }

    long long balance_factor(long long t) const noexcept {
        return node_height(nodes_[t].left) - node_height(nodes_[t].right);
    }

    template <typename U>
    long long new_node(U&& key) {
        if (!free_list_.empty()) {
            long long idx = free_list_.back();
            free_list_.pop_back();
            nodes_[idx].key = std::forward<U>(key);
            nodes_[idx].left = -1;
            nodes_[idx].right = -1;
            nodes_[idx].cnt = 1;
            nodes_[idx].size = 1;
            nodes_[idx].height = 1;
            return idx;
        }
        long long idx = static_cast<long long>(nodes_.size());
        nodes_.push_back(Node{std::forward<U>(key), -1, -1, 1, 1, 1});
        return idx;
    }

    void free_node(long long idx) {
        free_list_.push_back(idx);
        if constexpr (!std::is_trivially_destructible_v<T> &&
                      std::is_default_constructible_v<T> &&
                      std::is_move_assignable_v<T>) {
            nodes_[idx].key = T{};
        }
    }

    long long rotate_right(long long y) noexcept {
        long long x = nodes_[y].left;
        nodes_[y].left = nodes_[x].right;
        nodes_[x].right = y;
        pull(y);
        pull(x);
        return x;
    }

    long long rotate_left(long long x) noexcept {
        long long y = nodes_[x].right;
        nodes_[x].right = nodes_[y].left;
        nodes_[y].left = x;
        pull(x);
        pull(y);
        return y;
    }

    long long rebalance(long long t) noexcept {
        pull(t);
        long long bf = balance_factor(t);
        if (bf > 1) {
            if (balance_factor(nodes_[t].left) < 0) {
                nodes_[t].left = rotate_left(nodes_[t].left);
            }
            return rotate_right(t);
        }
        if (bf < -1) {
            if (balance_factor(nodes_[t].right) > 0) {
                nodes_[t].right = rotate_right(nodes_[t].right);
            }
            return rotate_left(t);
        }
        return t;
    }

    template <typename U>
    long long insert_impl(long long t, U&& key, bool& height_changed) {
        if (t == -1) {
            height_changed = true;
            return new_node(std::forward<U>(key));
        }
        if (comp_(key, nodes_[t].key)) {
            long long child =
                insert_impl(nodes_[t].left, std::forward<U>(key), height_changed);
            nodes_[t].left = child;
        } else if (comp_(nodes_[t].key, key)) {
            long long child =
                insert_impl(nodes_[t].right, std::forward<U>(key), height_changed);
            nodes_[t].right = child;
        } else {
            nodes_[t].cnt += 1;
            nodes_[t].size += 1;
            height_changed = false;
            return t;
        }
        if (!height_changed) {
            pull(t);
            return t;
        }
        return rebalance(t);
    }

    long long detach_min(long long t, long long& min_node) noexcept {
        if (nodes_[t].left == -1) {
            min_node = t;
            return nodes_[t].right;
        }
        nodes_[t].left = detach_min(nodes_[t].left, min_node);
        return rebalance(t);
    }

    long long remove_node(long long t) {
        if (nodes_[t].left == -1) {
            long long r = nodes_[t].right;
            free_node(t);
            return r;
        }
        if (nodes_[t].right == -1) {
            long long l = nodes_[t].left;
            free_node(t);
            return l;
        }
        long long min_node = -1;
        nodes_[t].right = detach_min(nodes_[t].right, min_node);
        nodes_[min_node].left = nodes_[t].left;
        nodes_[min_node].right = nodes_[t].right;
        free_node(t);
        return rebalance(min_node);
    }

    long long erase_impl(long long t, const T& key, bool& erased, bool& node_removed) {
        if (t == -1) {
            erased = false;
            node_removed = false;
            return -1;
        }
        if (comp_(key, nodes_[t].key)) {
            nodes_[t].left = erase_impl(nodes_[t].left, key, erased, node_removed);
        } else if (comp_(nodes_[t].key, key)) {
            nodes_[t].right = erase_impl(nodes_[t].right, key, erased, node_removed);
        } else {
            erased = true;
            if (nodes_[t].cnt > 1) {
                nodes_[t].cnt -= 1;
                nodes_[t].size -= 1;
                node_removed = false;
                return t;
            }
            node_removed = true;
            return remove_node(t);
        }
        if (!erased) return t;
        if (!node_removed) {
            pull(t);
            return t;
        }
        return rebalance(t);
    }

    long long erase_all_impl(long long t, const T& key, long long& removed_cnt) {
        if (t == -1) return -1;
        if (comp_(key, nodes_[t].key)) {
            nodes_[t].left = erase_all_impl(nodes_[t].left, key, removed_cnt);
        } else if (comp_(nodes_[t].key, key)) {
            nodes_[t].right = erase_all_impl(nodes_[t].right, key, removed_cnt);
        } else {
            removed_cnt = nodes_[t].cnt;
            return remove_node(t);
        }
        if (removed_cnt == 0) return t;
        return rebalance(t);
    }

    long long erase_kth_impl(
        long long t, long long k, std::optional<T>& removed, bool& node_removed) {
        long long left_sz = node_size(nodes_[t].left);
        if (k < left_sz) {
            nodes_[t].left = erase_kth_impl(nodes_[t].left, k, removed, node_removed);
        } else if (k >= left_sz + nodes_[t].cnt) {
            nodes_[t].right = erase_kth_impl(
                nodes_[t].right, k - left_sz - nodes_[t].cnt, removed, node_removed);
        } else {
            if (nodes_[t].cnt > 1) {
                removed.emplace(nodes_[t].key);
                nodes_[t].cnt -= 1;
                nodes_[t].size -= 1;
                node_removed = false;
                return t;
            }
            removed.emplace(std::move(nodes_[t].key));
            node_removed = true;
            return remove_node(t);
        }
        if (!node_removed) {
            pull(t);
            return t;
        }
        return rebalance(t);
    }

    const T* find_kth_ptr(long long k) const noexcept {
        if (k < 0 || k >= size()) return nullptr;
        long long cur = root_;
        while (cur != -1) {
            long long left_sz = node_size(nodes_[cur].left);
            if (k < left_sz) {
                cur = nodes_[cur].left;
            } else if (k >= left_sz + nodes_[cur].cnt) {
                k -= left_sz + nodes_[cur].cnt;
                cur = nodes_[cur].right;
            } else {
                return &nodes_[cur].key;
            }
        }
        return nullptr;
    }

    void inorder(long long t, std::vector<T>& out) const {
        if (t == -1) return;
        inorder(nodes_[t].left, out);
        for (long long i = 0; i < nodes_[t].cnt; ++i) {
            out.push_back(nodes_[t].key);
        }
        inorder(nodes_[t].right, out);
    }
};
