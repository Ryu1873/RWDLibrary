#pragma once

#include <cassert>
#include <concepts>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "hash_common.hpp"

namespace hash_detail {

struct PairFirstKey {
    template <typename Pair>
    constexpr const auto& operator()(const Pair& p) const noexcept {
        return p.first;
    }
};

}  // namespace hash_detail

/**
 * @brief Robin Hood ハッシュテーブル + 密配列格納による高速なハッシュマップ
 * @tparam Key キーの型 (デフォルト: long long)
 * @tparam Val 値の型 (デフォルト: long long)
 * @tparam Hash ハッシュ関数型 (デフォルト: FastHash)
 * @tparam KeyEqual キー等値比較関数型 (デフォルト: std::equal_to<Key>)
 * @note 挿入・削除・検索・添字アクセスを期待時間計算量 O(1)、全要素列挙を実要素数に対する
 *       厳密な O(N) で処理する。
 *       イテレーション順序は挿入・削除の履歴に依存し、未定義順序となる。
 *       イテレーション中に要素のキー (first) を直接書き換えないこと。
 *       挿入（再ハッシュ）および削除により既存のイテレータや参照は無効化される場合があるため、
 *       mp[a] = mp[b] のような同一マップへの再配置を伴う同時参照には注意すること。
 */
template <typename Key = long long,
          typename Val = long long,
          typename Hash = FastHash,
          typename KeyEqual = std::equal_to<Key>>
class HashMap {
private:
    using Table = hash_detail::DenseHashTable<
        Key, std::pair<Key, Val>, hash_detail::PairFirstKey, Hash, KeyEqual>;

public:
    using key_type = Key;
    using mapped_type = Val;
    using value_type = std::pair<Key, Val>;
    using size_type = long long;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using iterator = typename Table::iterator;
    using const_iterator = typename Table::const_iterator;

    /**
     * @brief 空の HashMap を構築する (初回挿入までメモリを確保しない)
     */
    HashMap() = default;

    /**
     * @brief 初期予定要素数を指定して空の HashMap を構築する
     * @param initial_capacity 事前確保する要素数 (initial_capacity >= 0)
     * @param hash ハッシュ関数
     * @param eq 等値比較関数
     */
    explicit HashMap(long long initial_capacity,
                     Hash hash = Hash(),
                     KeyEqual eq = KeyEqual())
        : table_(initial_capacity, std::move(hash), std::move(eq)) {}

    /**
     * @brief 指定したハッシュ関数・等値比較関数で空の HashMap を構築する
     */
    explicit HashMap(Hash hash, KeyEqual eq = KeyEqual())
        requires(!std::convertible_to<long long, Hash>)
        : table_(std::move(hash), std::move(eq)) {}

    /**
     * @brief 初期化リストから HashMap を構築する
     * @param init 初期 (キー, 値) ペア列
     * @param hash ハッシュ関数
     * @param eq 等値比較関数
     */
    HashMap(std::initializer_list<value_type> init,
            Hash hash = Hash(),
            KeyEqual eq = KeyEqual())
        : table_(static_cast<long long>(init.size()), std::move(hash), std::move(eq)) {
        for (const auto& kv : init) {
            insert(kv);
        }
    }

    /**
     * @brief イテレータ範囲 [first, last) から HashMap を構築する
     * @tparam InputIt 入力イテレータ型
     * @param first 先頭イテレータ
     * @param last 末尾イテレータ
     * @param hash ハッシュ関数
     * @param eq 等値比較関数
     */
    template <std::input_iterator InputIt>
    HashMap(InputIt first, InputIt last,
            Hash hash = Hash(),
            KeyEqual eq = KeyEqual())
        : table_(std::move(hash), std::move(eq)) {
        if constexpr (std::random_access_iterator<InputIt>) {
            reserve(static_cast<long long>(last - first));
        }
        insert(first, last);
    }

    /**
     * @brief 格納されているキーの個数を返す
     * @note 時間計算量: O(1)
     */
    long long size() const noexcept { return table_.size(); }

    /**
     * @brief マップが空かどうかを返す
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
     * @brief キー key がマップに含まれるかを判定する
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
     * @brief キー key の個数 (0 または 1) を返す
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
     * @brief キー key を検索し、見つかれば該当要素へのイテレータ、なければ end() を返す
     * @note 期待時間計算量: O(1)
     */
    iterator find(const Key& key) {
        const long long idx = table_.find_index(key);
        return idx == -1 ? end() : begin() + static_cast<std::ptrdiff_t>(idx);
    }

    template <typename K>
        requires hash_detail::valid_transparent_key<Key, Hash, KeyEqual, K>
    iterator find(const K& key) {
        const long long idx = table_.find_index(key);
        return idx == -1 ? end() : begin() + static_cast<std::ptrdiff_t>(idx);
    }

    /**
     * @brief キー key を検索し、見つかれば該当要素への const イテレータ、なければ end() を返す
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
     * @brief キー key に対応する値への参照を返す (存在しない場合はデフォルト値 Val{} を挿入する)
     * @note 期待時間計算量: 償却 O(1)
     */
    Val& operator[](const Key& key) {
        return try_emplace(key).first->second;
    }

    /**
     * @brief キー key をムーブで用いて対応する値への参照を返す (存在しない場合は Val{} を挿入する)
     * @note 期待時間計算量: 償却 O(1)
     */
    Val& operator[](Key&& key) {
        return try_emplace(std::move(key)).first->second;
    }

    template <typename K>
        requires hash_detail::valid_transparent_key<Key, Hash, KeyEqual, K> &&
                 std::constructible_from<Key, const K&>
    Val& operator[](const K& key) {
        return try_emplace(key).first->second;
    }

    /**
     * @brief キー key に対応する値への参照を返す (存在しない場合は std::out_of_range を送出する)
     * @note 期待時間計算量: O(1)
     */
    Val& at(const Key& key) {
        const long long idx = table_.find_index(key);
        if (idx == -1) throw std::out_of_range("HashMap::at: key not found");
        return (begin() + static_cast<std::ptrdiff_t>(idx))->second;
    }

    template <typename K>
        requires hash_detail::valid_transparent_key<Key, Hash, KeyEqual, K>
    Val& at(const K& key) {
        const long long idx = table_.find_index(key);
        if (idx == -1) throw std::out_of_range("HashMap::at: key not found");
        return (begin() + static_cast<std::ptrdiff_t>(idx))->second;
    }

    /**
     * @brief キー key に対応する値への const 参照を返す (存在しない場合は std::out_of_range を送出する)
     * @note 期待時間計算量: O(1)
     */
    const Val& at(const Key& key) const {
        const long long idx = table_.find_index(key);
        if (idx == -1) throw std::out_of_range("HashMap::at: key not found");
        return (begin() + static_cast<std::ptrdiff_t>(idx))->second;
    }

    template <typename K>
        requires hash_detail::valid_transparent_key<Key, Hash, KeyEqual, K>
    const Val& at(const K& key) const {
        const long long idx = table_.find_index(key);
        if (idx == -1) throw std::out_of_range("HashMap::at: key not found");
        return (begin() + static_cast<std::ptrdiff_t>(idx))->second;
    }

    /**
     * @brief キー key に対応する値を返す (存在しない場合は挿入せず default_val を返す)
     * @note 期待時間計算量: O(1)
     */
    Val get_or(const Key& key, const Val& default_val) const {
        const long long idx = table_.find_index(key);
        return idx == -1 ? default_val
                         : (begin() + static_cast<std::ptrdiff_t>(idx))->second;
    }

    template <typename K>
        requires hash_detail::valid_transparent_key<Key, Hash, KeyEqual, K>
    Val get_or(const K& key, const Val& default_val) const {
        const long long idx = table_.find_index(key);
        return idx == -1 ? default_val
                         : (begin() + static_cast<std::ptrdiff_t>(idx))->second;
    }

    /**
     * @brief キー key に対応する値を返す (存在しない場合は挿入せず Val{} を返す)
     * @note 期待時間計算量: O(1)
     */
    Val get_or(const Key& key) const
        requires std::default_initializable<Val> {
        const long long idx = table_.find_index(key);
        return idx == -1 ? Val{}
                         : (begin() + static_cast<std::ptrdiff_t>(idx))->second;
    }

    template <typename K>
        requires hash_detail::valid_transparent_key<Key, Hash, KeyEqual, K> &&
                 std::default_initializable<Val>
    Val get_or(const K& key) const {
        const long long idx = table_.find_index(key);
        return idx == -1 ? Val{}
                         : (begin() + static_cast<std::ptrdiff_t>(idx))->second;
    }

    /**
     * @brief (キー, 値) ペアを挿入する (既にキーが存在する場合は上書きしない)
     * @return {要素を指すイテレータ, 新規に挿入された場合 true}
     * @note 期待時間計算量: 償却 O(1)
     */
    std::pair<iterator, bool> insert(const value_type& kv) {
        return table_.emplace_with_key(kv.first, kv);
    }

    /**
     * @brief (キー, 値) ペアをムーブして挿入する (既にキーが存在する場合は上書きしない)
     * @return {要素を指すイテレータ, 新規に挿入された場合 true}
     * @note 期待時間計算量: 償却 O(1)
     */
    std::pair<iterator, bool> insert(value_type&& kv) {
        return table_.emplace_with_key(kv.first, std::move(kv));
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
    void insert(std::initializer_list<value_type> init) {
        for (const auto& kv : init) {
            insert(kv);
        }
    }

    /**
     * @brief キー key が存在しなければ (key, val) を挿入し、既に存在すれば値を val で上書きする
     * @return {要素を指すイテレータ, 新規に挿入された場合 true}
     * @note 期待時間計算量: 償却 O(1)
     */
    template <typename M>
    std::pair<iterator, bool> insert_or_assign(const Key& key, M&& val) {
        auto res = try_emplace(key, std::forward<M>(val));
        if (!res.second) {
            res.first->second = std::forward<M>(val);
        }
        return res;
    }

    /**
     * @brief キー key をムーブして挿入、または既存値を val で上書きする
     * @return {要素を指すイテレータ, 新規に挿入された場合 true}
     * @note 期待時間計算量: 償却 O(1)
     */
    template <typename M>
    std::pair<iterator, bool> insert_or_assign(Key&& key, M&& val) {
        auto res = try_emplace(std::move(key), std::forward<M>(val));
        if (!res.second) {
            res.first->second = std::forward<M>(val);
        }
        return res;
    }

    /**
     * @brief キー key が存在しない場合のみ args... から値を直接構築して挿入する
     * @return {要素を指すイテレータ, 新規に挿入された場合 true}
     * @note 期待時間計算量: 償却 O(1)
     */
    template <typename... Args>
    std::pair<iterator, bool> try_emplace(const Key& key, Args&&... args) {
        return table_.emplace_with_key(
            key,
            std::piecewise_construct,
            std::forward_as_tuple(key),
            std::forward_as_tuple(std::forward<Args>(args)...));
    }

    /**
     * @brief キー key が存在しない場合のみ key をムーブし args... から値を直接構築して挿入する
     * @return {要素を指すイテレータ, 新規に挿入された場合 true}
     * @note 期待時間計算量: 償却 O(1)
     */
    template <typename... Args>
    std::pair<iterator, bool> try_emplace(Key&& key, Args&&... args) {
        return table_.emplace_with_key(
            key,
            std::piecewise_construct,
            std::forward_as_tuple(std::move(key)),
            std::forward_as_tuple(std::forward<Args>(args)...));
    }

    template <typename K, typename... Args>
        requires hash_detail::valid_transparent_key<Key, Hash, KeyEqual, K> &&
                 std::constructible_from<Key, const K&>
    std::pair<iterator, bool> try_emplace(const K& key, Args&&... args) {
        return table_.emplace_with_key(
            key,
            std::piecewise_construct,
            std::forward_as_tuple(key),
            std::forward_as_tuple(std::forward<Args>(args)...));
    }

    /**
     * @brief 引数から (キー, 値) ペアを直接構築して挿入する (既存キーは上書きしない)
     * @return {要素を指すイテレータ, 新規に挿入された場合 true}
     * @note 期待時間計算量: 償却 O(1)
     */
    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        if constexpr (sizeof...(Args) == 2) {
            return emplace_pair(std::forward<Args>(args)...);
        } else {
            value_type kv(std::forward<Args>(args)...);
            return insert(std::move(kv));
        }
    }

    /**
     * @brief キー key を削除する (存在しない場合は何もしない)
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
     *       ループ中に it = mp.erase(it) で全要素を漏れなく 1 回ずつ走査できる。
     *       期待時間計算量: O(1)
     */
    iterator erase(const_iterator pos) {
        return table_.erase_pos(pos);
    }

    /**
     * @brief 他の HashMap と内容を交換する
     */
    void swap(HashMap& other) noexcept(noexcept(table_.swap(other.table_))) {
        table_.swap(other.table_);
    }

    /**
     * @brief 内部の密配列への const 参照を返す
     */
    const std::vector<value_type>& values() const noexcept { return table_.values(); }

    iterator begin() noexcept { return table_.begin(); }
    iterator end() noexcept { return table_.end(); }
    const_iterator begin() const noexcept { return table_.begin(); }
    const_iterator end() const noexcept { return table_.end(); }
    const_iterator cbegin() const noexcept { return table_.cbegin(); }
    const_iterator cend() const noexcept { return table_.cend(); }

    const hasher& hash_function() const noexcept { return table_.hash_function(); }
    const key_equal& key_eq() const noexcept { return table_.key_eq(); }

    friend bool operator==(const HashMap& lhs, const HashMap& rhs) {
        if (&lhs == &rhs) return true;
        if (lhs.size() != rhs.size()) return false;
        for (const auto& [k, v] : lhs) {
            const auto it = rhs.find(k);
            if (it == rhs.end() || !(it->second == v)) return false;
        }
        return true;
    }

private:
    Table table_;

    template <typename A, typename B>
    std::pair<iterator, bool> emplace_pair(A&& a, B&& b) {
        if constexpr (std::is_same_v<std::remove_cvref_t<A>, Key> ||
                      (hash_detail::valid_transparent_key<Key, Hash, KeyEqual, A> &&
                       std::constructible_from<Key, A&&>)) {
            return table_.emplace_with_key(
                a,
                std::piecewise_construct,
                std::forward_as_tuple(std::forward<A>(a)),
                std::forward_as_tuple(std::forward<B>(b)));
        } else {
            value_type kv(std::forward<A>(a), std::forward<B>(b));
            return insert(std::move(kv));
        }
    }
};
