#pragma once

#include <cassert>
#include <concepts>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

#include "hash_common.hpp"

namespace hash_detail {

struct IdentityKey {
    template <typename T>
    constexpr const T& operator()(const T& x) const noexcept {
        return x;
    }
};

}  // namespace hash_detail

/**
 * @brief Robin Hood ハッシュテーブル + 密配列格納による高速なハッシュ集合
 * @tparam Key 要素の型 (デフォルト: long long)
 * @tparam Hash ハッシュ関数型 (デフォルト: FastHash)
 * @tparam KeyEqual 等値比較関数型 (デフォルト: std::equal_to<Key>)
 * @note 挿入・削除・存在判定を期待時間計算量 O(1)、全要素列挙を実要素数に対する厳密な O(N) で処理する。
 *       イテレーション順序は挿入・削除の履歴に依存し、未定義順序となる。
 *       挿入（再ハッシュ）および削除により既存のイテレータや参照は無効化される場合がある。
 */
template <typename Key = long long,
          typename Hash = FastHash,
          typename KeyEqual = std::equal_to<Key>>
class HashSet {
private:
    using Table = hash_detail::DenseHashTable<
        Key, Key, hash_detail::IdentityKey, Hash, KeyEqual>;

public:
    using key_type = Key;
    using value_type = Key;
    using size_type = long long;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using iterator = typename Table::const_iterator;
    using const_iterator = typename Table::const_iterator;

    /**
     * @brief 空の HashSet を構築する (初回挿入までメモリを確保しない)
     */
    HashSet() = default;

    /**
     * @brief 初期予定要素数を指定して空の HashSet を構築する
     * @param initial_capacity 事前確保する要素数 (initial_capacity >= 0)
     * @param hash ハッシュ関数
     * @param eq 等値比較関数
     */
    explicit HashSet(long long initial_capacity,
                     Hash hash = Hash(),
                     KeyEqual eq = KeyEqual())
        : table_(initial_capacity, std::move(hash), std::move(eq)) {}

    /**
     * @brief 指定したハッシュ関数・等値比較関数で空の HashSet を構築する
     */
    explicit HashSet(Hash hash, KeyEqual eq = KeyEqual())
        requires(!std::convertible_to<long long, Hash>)
        : table_(std::move(hash), std::move(eq)) {}

    /**
     * @brief 初期化リストから HashSet を構築する
     * @param init 初期要素列
     * @param hash ハッシュ関数
     * @param eq 等値比較関数
     */
    HashSet(std::initializer_list<Key> init,
            Hash hash = Hash(),
            KeyEqual eq = KeyEqual())
        : table_(static_cast<long long>(init.size()), std::move(hash), std::move(eq)) {
        for (const auto& x : init) {
            insert(x);
        }
    }

    /**
     * @brief イテレータ範囲 [first, last) から HashSet を構築する
     * @tparam InputIt 入力イテレータ型
     * @param first 先頭イテレータ
     * @param last 末尾イテレータ
     * @param hash ハッシュ関数
     * @param eq 等値比較関数
     */
    template <std::input_iterator InputIt>
    HashSet(InputIt first, InputIt last,
            Hash hash = Hash(),
            KeyEqual eq = KeyEqual())
        : table_(std::move(hash), std::move(eq)) {
        if constexpr (std::random_access_iterator<InputIt>) {
            reserve(static_cast<long long>(last - first));
        }
        insert(first, last);
    }

    /**
     * @brief 格納されている要素数を返す
     * @note 時間計算量: O(1)
     */
    long long size() const noexcept { return table_.size(); }

    /**
     * @brief 集合が空かどうかを返す
     * @note 時間計算量: O(1)
     */
    bool empty() const noexcept { return table_.empty(); }

    /**
     * @brief 少なくとも n 要素を格納できるようバケットを事前確保する
     * @param n 確保する要素数 (n >= 0)
     */
    void reserve(long long n) { table_.reserve(n); }

    /**
     * @brief すべての要素を削除して空にする (確保済みメモリは保持する)
     * @note 時間計算量: 格納要素数 N とバケット数 B に対し O(min(N, B))
     */
    void clear() noexcept { table_.clear(); }

    /**
     * @brief 余分な確保済みメモリを解放する
     */
    void shrink_to_fit() { table_.shrink_to_fit(); }

    /**
     * @brief 要素 key が集合に含まれるかを判定する
     * @note 期待時間計算量: O(1)
     */
    bool contains(const Key& key) const {
        return table_.find_index(key) != -1;
    }

    template <typename K>
        requires hash_detail::valid_transparent_key<Key, Hash, KeyEqual, K>
    bool contains(const K& key) const {
        return table_.find_index(key) != -1;
    }

    /**
     * @brief 要素 key の個数 (0 または 1) を返す
     * @note 期待時間計算量: O(1)
     */
    long long count(const Key& key) const {
        return contains(key) ? 1LL : 0LL;
    }

    template <typename K>
        requires hash_detail::valid_transparent_key<Key, Hash, KeyEqual, K>
    long long count(const K& key) const {
        return contains(key) ? 1LL : 0LL;
    }

    /**
     * @brief 要素 key を検索し、見つかれば該当要素へのイテレータ、なければ end() を返す
     * @note 期待時間計算量: O(1)
     */
    const_iterator find(const Key& key) const {
        const long long idx = table_.find_index(key);
        return idx == -1 ? end() : begin() + static_cast<std::ptrdiff_t>(idx);
    }

    template <typename K>
        requires hash_detail::valid_transparent_key<Key, Hash, KeyEqual, K>
    const_iterator find(const K& key) const {
        const long long idx = table_.find_index(key);
        return idx == -1 ? end() : begin() + static_cast<std::ptrdiff_t>(idx);
    }

    /**
     * @brief 要素 key を挿入する (既に存在する場合は挿入しない)
     * @return {要素を指すイテレータ, 新規に挿入された場合 true}
     * @note 期待時間計算量: 償却 O(1)
     */
    std::pair<const_iterator, bool> insert(const Key& key) {
        return table_.emplace_with_key(key, key);
    }

    /**
     * @brief 要素 key をムーブして挿入する (既に存在する場合は挿入しない)
     * @return {要素を指すイテレータ, 新規に挿入された場合 true}
     * @note 期待時間計算量: 償却 O(1)
     */
    std::pair<const_iterator, bool> insert(Key&& key) {
        return table_.emplace_with_key(key, std::move(key));
    }

    /**
     * @brief イテレータ範囲 [first, last) の要素をすべて挿入する
     */
    template <std::input_iterator InputIt>
    void insert(InputIt first, InputIt last) {
        for (; first != last; ++first) {
            insert(*first);
        }
    }

    /**
     * @brief 初期化リストの要素をすべて挿入する
     */
    void insert(std::initializer_list<Key> init) {
        for (const auto& x : init) {
            insert(x);
        }
    }

    /**
     * @brief 引数から要素を直接構築して挿入する
     * @return {要素を指すイテレータ, 新規に挿入された場合 true}
     * @note 期待時間計算量: 償却 O(1)
     */
    template <typename... Args>
    std::pair<const_iterator, bool> emplace(Args&&... args) {
        if constexpr (sizeof...(Args) == 1) {
            return emplace_single(std::forward<Args>(args)...);
        } else {
            Key val(std::forward<Args>(args)...);
            return insert(std::move(val));
        }
    }

    /**
     * @brief 要素 key を削除する (存在しない場合は何もしない)
     * @return 削除された場合 true、存在しなかった場合 false
     * @note 期待時間計算量: O(1)
     */
    bool erase(const Key& key) {
        return table_.erase_key(key);
    }

    template <typename K>
        requires hash_detail::valid_transparent_key<Key, Hash, KeyEqual, K>
    bool erase(const K& key) {
        return table_.erase_key(key);
    }

    /**
     * @brief イテレータ pos が指す要素を削除し、次の有効な位置のイテレータを返す
     * @note swap-and-pop を用いるため末尾以外の要素順が入れ替わるが、
     *       ループ中に it = st.erase(it) で全要素を漏れなく 1 回ずつ走査できる。
     *       期待時間計算量: O(1)
     */
    const_iterator erase(const_iterator pos) {
        return table_.erase_pos(pos);
    }

    /**
     * @brief 他の HashSet と内容を交換する
     */
    void swap(HashSet& other) noexcept(noexcept(table_.swap(other.table_))) {
        table_.swap(other.table_);
    }

    /**
     * @brief 内部の密配列への const 参照を返す
     */
    const std::vector<Key>& values() const noexcept { return table_.values(); }

    const_iterator begin() const noexcept { return table_.cbegin(); }
    const_iterator end() const noexcept { return table_.cend(); }
    const_iterator cbegin() const noexcept { return table_.cbegin(); }
    const_iterator cend() const noexcept { return table_.cend(); }

    const hasher& hash_function() const noexcept { return table_.hash_function(); }
    const key_equal& key_eq() const noexcept { return table_.key_eq(); }

    friend bool operator==(const HashSet& lhs, const HashSet& rhs) {
        if (&lhs == &rhs) return true;
        if (lhs.size() != rhs.size()) return false;
        for (const auto& k : lhs) {
            if (!rhs.contains(k)) return false;
        }
        return true;
    }

private:
    Table table_;

    template <typename Arg>
    std::pair<const_iterator, bool> emplace_single(Arg&& arg) {
        if constexpr (std::is_same_v<std::remove_cvref_t<Arg>, Key> ||
                      (hash_detail::valid_transparent_key<Key, Hash, KeyEqual, Arg> &&
                       std::constructible_from<Key, Arg&&>)) {
            return table_.emplace_with_key(arg, std::forward<Arg>(arg));
        } else {
            Key val(std::forward<Arg>(arg));
            return insert(std::move(val));
        }
    }
};
