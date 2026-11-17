#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <chrono>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <limits>
#include <optional>
#include <random>
#include <ranges>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace hash_detail {

inline std::uint64_t splitmix64(std::uint64_t x) noexcept {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

inline std::uint64_t wymix(std::uint64_t a, std::uint64_t b) noexcept {
#ifdef __SIZEOF_INT128__
    const __uint128_t r = static_cast<__uint128_t>(a) * static_cast<__uint128_t>(b);
    return static_cast<std::uint64_t>(r) ^ static_cast<std::uint64_t>(r >> 64);
#else
    const std::uint64_t a_lo = a & 0xFFFFFFFFULL;
    const std::uint64_t a_hi = a >> 32;
    const std::uint64_t b_lo = b & 0xFFFFFFFFULL;
    const std::uint64_t b_hi = b >> 32;
    const std::uint64_t p00 = a_lo * b_lo;
    const std::uint64_t p01 = a_lo * b_hi;
    const std::uint64_t p10 = a_hi * b_lo;
    const std::uint64_t p11 = a_hi * b_hi;
    const std::uint64_t mid =
        (p00 >> 32) + (p01 & 0xFFFFFFFFULL) + (p10 & 0xFFFFFFFFULL);
    const std::uint64_t lo = (p00 & 0xFFFFFFFFULL) | (mid << 32);
    const std::uint64_t hi = p11 + (p01 >> 32) + (p10 >> 32) + (mid >> 32);
    return lo ^ hi;
#endif
}

inline std::uint64_t hash_seed() noexcept {
    static const std::uint64_t seed = []() noexcept {
        std::uint64_t x = static_cast<std::uint64_t>(
            std::chrono::steady_clock::now().time_since_epoch().count());
        x ^= static_cast<std::uint64_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count());
        x ^= reinterpret_cast<std::uintptr_t>(&x);
        try {
            std::random_device rd;
            x ^= (static_cast<std::uint64_t>(rd()) << 32) ^
                 static_cast<std::uint64_t>(rd());
        } catch (...) {
            // random_device が利用できない環境では時刻とアドレスの混合値のみを使用する
        }
        return splitmix64(x);
    }();
    return seed;
}

inline std::uint64_t combine_hash(std::uint64_t h1, std::uint64_t h2) noexcept {
    return splitmix64((h1 + 0x9e3779b97f4a7c15ULL) ^ h2);
}

inline std::uint64_t hash_bytes(const char* data, std::size_t len, std::uint64_t seed) noexcept {
    std::uint64_t h = seed ^ static_cast<std::uint64_t>(len);
    std::size_t i = 0;
    while (i + 8 <= len) {
        std::uint64_t chunk = 0;
        std::memcpy(&chunk, data + i, 8);
        h = wymix(h ^ chunk, 0x9e3779b97f4a7c15ULL);
        i += 8;
    }
    if (i < len) {
        std::uint64_t tail = 0;
        std::memcpy(&tail, data + i, len - i);
        h = wymix(h ^ tail, 0x9e3779b97f4a7c15ULL);
    }
    return splitmix64(h ^ static_cast<std::uint64_t>(len));
}

template <typename T>
struct is_std_pair : std::false_type {};
template <typename A, typename B>
struct is_std_pair<std::pair<A, B>> : std::true_type {};

template <typename T>
struct is_std_tuple : std::false_type {};
template <typename... Ts>
struct is_std_tuple<std::tuple<Ts...>> : std::true_type {};

template <typename T>
struct is_std_optional : std::false_type {};
template <typename T>
struct is_std_optional<std::optional<T>> : std::true_type {};

template <typename T>
struct is_std_variant : std::false_type {};
template <typename... Ts>
struct is_std_variant<std::variant<Ts...>> : std::true_type {};

template <typename T>
inline constexpr bool is_int128_v =
#ifdef __SIZEOF_INT128__
    std::is_same_v<T, __int128_t> || std::is_same_v<T, __uint128_t>;
#else
    false;
#endif

template <typename Hash>
concept has_is_avalanching = requires { typename Hash::is_avalanching; };

template <typename Hash, typename KeyEqual>
concept has_transparent_lookup =
    requires {
        typename Hash::is_transparent;
        typename KeyEqual::is_transparent;
    };

}  // namespace hash_detail

/**
 * @brief 実行時ランダムシード付きの高速な汎用ハッシュ関数オブジェクト
 * @note 整数・浮動小数点・文字列・pair・tuple・optional・variant・コンテナ等の主要な型を
 *       そのままハッシュでき、対ハッシュ衝突攻撃（ハック）を防止する。
 *       ユーザー独自の型に対しては std::hash<T> が定義されていればフォールバックして使用する。
 */
struct FastHash {
    using is_avalanching = void;

    FastHash() noexcept : seed_(hash_detail::hash_seed()) {}
    explicit FastHash(std::uint64_t seed) noexcept : seed_(seed) {}

    template <typename T>
    std::uint64_t operator()(const T& x) const noexcept {
        using U = std::remove_cvref_t<T>;
        const std::uint64_t seed = seed_;

        if constexpr (hash_detail::is_int128_v<U>) {
#ifdef __SIZEOF_INT128__
            const __uint128_t u = static_cast<__uint128_t>(x);
            const std::uint64_t lo = static_cast<std::uint64_t>(u);
            const std::uint64_t hi = static_cast<std::uint64_t>(u >> 64);
            return hash_detail::combine_hash(hash_detail::splitmix64(lo ^ seed), hi);
#endif
        } else if constexpr (std::is_integral_v<U>) {
            return hash_detail::splitmix64(static_cast<std::uint64_t>(x) ^ seed);
        } else if constexpr (std::is_enum_v<U>) {
            using Under = std::underlying_type_t<U>;
            return (*this)(static_cast<Under>(x));
        } else if constexpr (std::is_floating_point_v<U>) {
            // -0.0 と +0.0 は等値比較で同値なため同じハッシュ値へ正規化する
            const U norm = (x == U{0}) ? U{0} : x;
            if constexpr (sizeof(U) == 4) {
                return hash_detail::splitmix64(
                    static_cast<std::uint64_t>(std::bit_cast<std::uint32_t>(norm)) ^ seed);
            } else if constexpr (sizeof(U) == 8) {
                return hash_detail::splitmix64(
                    std::bit_cast<std::uint64_t>(norm) ^ seed);
            } else {
                return hash_detail::splitmix64(
                    static_cast<std::uint64_t>(std::hash<U>{}(norm)) ^ seed);
            }
        } else if constexpr (std::is_same_v<U, std::nullptr_t>) {
            return hash_detail::splitmix64(seed);
        } else if constexpr (std::is_pointer_v<U>) {
            return hash_detail::splitmix64(
                static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(x)) ^ seed);
        } else if constexpr (std::is_same_v<U, std::string> ||
                             std::is_same_v<U, std::string_view>) {
            return hash_detail::hash_bytes(x.data(), x.size(), seed);
        } else if constexpr (hash_detail::is_std_pair<U>::value) {
            return hash_detail::combine_hash((*this)(x.first), (*this)(x.second));
        } else if constexpr (hash_detail::is_std_tuple<U>::value) {
            return std::apply(
                [this, seed](const auto&... elems) {
                    std::uint64_t h = seed ^ static_cast<std::uint64_t>(sizeof...(elems));
                    ((h = hash_detail::combine_hash(h, (*this)(elems))), ...);
                    return h;
                },
                x);
        } else if constexpr (hash_detail::is_std_optional<U>::value) {
            if (!x.has_value()) {
                return hash_detail::splitmix64(seed ^ 0x517cc1b727220a95ULL);
            }
            return hash_detail::splitmix64((*this)(*x) ^ 0x9e3779b97f4a7c15ULL);
        } else if constexpr (hash_detail::is_std_variant<U>::value) {
            return std::visit(
                [this, &x, seed](const auto& val) {
                    const std::uint64_t idx_hash = hash_detail::splitmix64(
                        static_cast<std::uint64_t>(x.index()) ^ seed);
                    return hash_detail::combine_hash(idx_hash, (*this)(val));
                },
                x);
        } else if constexpr (std::is_same_v<U, std::vector<bool>>) {
            std::uint64_t h = seed ^ static_cast<std::uint64_t>(x.size());
            std::uint64_t acc = 0;
            std::size_t bit_pos = 0;
            for (bool b : x) {
                if (b) acc |= (1ULL << bit_pos);
                ++bit_pos;
                if (bit_pos == 64) {
                    h = hash_detail::wymix(h ^ acc, 0x9e3779b97f4a7c15ULL);
                    acc = 0;
                    bit_pos = 0;
                }
            }
            if (bit_pos > 0) {
                h = hash_detail::wymix(h ^ acc, 0x9e3779b97f4a7c15ULL);
            }
            return hash_detail::splitmix64(h ^ static_cast<std::uint64_t>(x.size()));
        } else if constexpr (std::ranges::input_range<const U>) {
            std::uint64_t h = seed;
            std::uint64_t cnt = 0;
            for (const auto& elem : x) {
                h = hash_detail::wymix(h ^ (*this)(elem), 0x9e3779b97f4a7c15ULL);
                ++cnt;
            }
            return hash_detail::splitmix64(h ^ cnt);
        } else if constexpr (requires(const U& u) { std::hash<U>{}(u); }) {
            return hash_detail::splitmix64(
                static_cast<std::uint64_t>(std::hash<U>{}(x)) ^ seed);
        } else {
            static_assert(
                sizeof(U) == 0,
                "FastHash: unsupported key type; specialize std::hash<T> or pass a custom Hash.");
            return 0;
        }
    }

private:
    std::uint64_t seed_;
};

namespace hash_detail {

template <typename Key, typename Hash, typename KeyEqual, typename K>
concept transparent_string_key =
    std::is_same_v<Key, std::string> &&
    std::is_same_v<Hash, FastHash> &&
    std::is_same_v<KeyEqual, std::equal_to<Key>> &&
    !std::is_same_v<std::remove_cvref_t<K>, Key> &&
    std::is_convertible_v<const K&, std::string_view>;

template <typename Key, typename Hash, typename KeyEqual, typename K>
concept transparent_custom_key =
    has_transparent_lookup<Hash, KeyEqual> &&
    !std::is_same_v<std::remove_cvref_t<K>, Key> &&
    requires(const Hash& h, const KeyEqual& eq, const Key& a, const K& b) {
        h(b);
        { eq(a, b) } -> std::convertible_to<bool>;
    };

template <typename Key, typename Hash, typename KeyEqual, typename K>
concept valid_transparent_key =
    transparent_string_key<Key, Hash, KeyEqual, K> ||
    transparent_custom_key<Key, Hash, KeyEqual, K>;

/**
 * @brief Robin Hood ハッシュテーブル + 密配列格納の共通エンジン
 * @tparam Key キー型
 * @tparam ValueType 密配列に格納する要素型 (Set は Key、Map は std::pair<Key, Val>)
 * @tparam KeyOfValue ValueType から const Key& を取り出す関数オブジェクト
 * @tparam Hash ハッシュ関数オブジェクト型
 * @tparam KeyEqual キー等値比較関数オブジェクト型
 */
template <typename Key, typename ValueType, typename KeyOfValue, typename Hash, typename KeyEqual>
class DenseHashTable {
public:
    using key_type = Key;
    using value_type = ValueType;
    using size_type = long long;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using iterator = typename std::vector<ValueType>::iterator;
    using const_iterator = typename std::vector<ValueType>::const_iterator;

    DenseHashTable() = default;

    explicit DenseHashTable(Hash hash, KeyEqual eq = KeyEqual())
        : hash_(std::move(hash)), eq_(std::move(eq)) {}

    explicit DenseHashTable(long long initial_capacity,
                            Hash hash = Hash(),
                            KeyEqual eq = KeyEqual())
        : hash_(std::move(hash)), eq_(std::move(eq)) {
        reserve(initial_capacity);
    }

    DenseHashTable(const DenseHashTable&) = default;
    DenseHashTable& operator=(const DenseHashTable&) = default;

    DenseHashTable(DenseHashTable&& other) noexcept(
        std::is_nothrow_move_constructible_v<std::vector<ValueType>> &&
        std::is_nothrow_move_constructible_v<std::vector<std::uint64_t>> &&
        std::is_nothrow_move_constructible_v<Hash> &&
        std::is_nothrow_move_constructible_v<KeyEqual>)
        : values_(std::move(other.values_)),
          buckets_(std::move(other.buckets_)),
          max_size_(std::exchange(other.max_size_, 0)),
          shift_(std::exchange(other.shift_, 0)),
          hash_(std::move(other.hash_)),
          eq_(std::move(other.eq_)) {}

    DenseHashTable& operator=(DenseHashTable&& other) noexcept(
        std::is_nothrow_move_assignable_v<std::vector<ValueType>> &&
        std::is_nothrow_move_assignable_v<std::vector<std::uint64_t>> &&
        std::is_nothrow_move_assignable_v<Hash> &&
        std::is_nothrow_move_assignable_v<KeyEqual>) {
        if (this != &other) {
            values_ = std::move(other.values_);
            buckets_ = std::move(other.buckets_);
            max_size_ = std::exchange(other.max_size_, 0);
            shift_ = std::exchange(other.shift_, 0);
            hash_ = std::move(other.hash_);
            eq_ = std::move(other.eq_);
        }
        return *this;
    }

    long long size() const noexcept {
        return static_cast<long long>(values_.size());
    }

    bool empty() const noexcept {
        return values_.empty();
    }

    void reserve(long long n) {
        assert(n >= 0);
        if (n == 0) return;
        const std::size_t need = calc_buckets(static_cast<std::size_t>(n));
        if (need > buckets_.size()) {
            rehash_impl(need);
        }
    }

    void clear() noexcept {
        if (values_.empty()) return;
        // 格納要素数がバケット数に対して十分少ない場合は個別消去して O(size) で初期化する
        if constexpr (noexcept(hash_key(std::declval<const Key&>()))) {
            if (values_.size() * 8 < buckets_.size()) {
                for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(values_.size()); ++i) {
                    erase_bucket_only(i);
                }
                values_.clear();
                return;
            }
        }
        values_.clear();
        std::fill(buckets_.begin(), buckets_.end(), 0ULL);
    }

    void shrink_to_fit() {
        if (values_.empty()) {
            std::vector<ValueType>().swap(values_);
            std::vector<std::uint64_t>().swap(buckets_);
            max_size_ = 0;
            shift_ = 0;
            return;
        }
        const std::size_t need = calc_buckets(values_.size());
        if (need < buckets_.size()) {
            rehash_impl(need);
        }
        values_.shrink_to_fit();
    }

    template <typename K>
    long long find_index(const K& key) const {
        if (values_.empty()) return -1;
        const std::uint64_t h = hash_key(key);
        std::uint64_t dist_fp = make_dist_fp(h);
        std::size_t pos = h >> shift_;
        const std::size_t mask = buckets_.size() - 1;
        std::uint64_t b = buckets_[pos];
        while (dist_fp <= b) {
            if ((b & UPPER_MASK) == dist_fp &&
                eq_key(KeyOfValue{}(values_[b & IDX_MASK]), key)) {
                return static_cast<long long>(b & IDX_MASK);
            }
            dist_fp += DIST_INC;
            pos = (pos + 1) & mask;
            b = buckets_[pos];
        }
        return -1;
    }

    template <typename K, typename... Args>
    std::pair<iterator, bool> emplace_with_key(const K& key, Args&&... args) {
        if (buckets_.empty()) {
            rehash_impl(MIN_BUCKETS);
        }
        const std::uint64_t h = hash_key(key);
        std::uint64_t dist_fp = make_dist_fp(h);
        std::size_t pos = h >> shift_;
        const std::size_t mask = buckets_.size() - 1;
        std::uint64_t b = buckets_[pos];
        while (dist_fp <= b) {
            if ((b & UPPER_MASK) == dist_fp &&
                eq_key(KeyOfValue{}(values_[b & IDX_MASK]), key)) {
                return {values_.begin() + static_cast<std::ptrdiff_t>(b & IDX_MASK), false};
            }
            dist_fp += DIST_INC;
            pos = (pos + 1) & mask;
            b = buckets_[pos];
        }
        if (values_.size() >= max_size_) {
            rehash_impl(buckets_.size() * 2);
            return emplace_fresh_with_hash(h, std::forward<Args>(args)...);
        }
        const std::uint32_t new_idx = static_cast<std::uint32_t>(values_.size());
        assert(values_.size() < (1ULL << 32));
        values_.emplace_back(std::forward<Args>(args)...);
        place_and_shift(pos, dist_fp | new_idx);
        return {values_.begin() + static_cast<std::ptrdiff_t>(new_idx), true};
    }

    template <typename K>
    bool erase_key(const K& key) {
        if (values_.empty()) return false;
        const std::uint64_t h = hash_key(key);
        std::uint64_t dist_fp = make_dist_fp(h);
        std::size_t pos = h >> shift_;
        const std::size_t mask = buckets_.size() - 1;
        std::uint64_t b = buckets_[pos];
        while (dist_fp <= b) {
            if ((b & UPPER_MASK) == dist_fp &&
                eq_key(KeyOfValue{}(values_[b & IDX_MASK]), key)) {
                erase_at(pos, static_cast<std::uint32_t>(b & IDX_MASK));
                return true;
            }
            dist_fp += DIST_INC;
            pos = (pos + 1) & mask;
            b = buckets_[pos];
        }
        return false;
    }

    iterator erase_pos(const_iterator pos) {
        assert(cbegin() <= pos && pos < cend());
        const std::uint32_t val_idx = static_cast<std::uint32_t>(pos - cbegin());
        const std::size_t bucket_pos =
            find_bucket_by_idx(val_idx, KeyOfValue{}(values_[val_idx]));
        erase_at(bucket_pos, val_idx);
        return values_.begin() + static_cast<std::ptrdiff_t>(val_idx);
    }

    void swap(DenseHashTable& other) noexcept(
        std::is_nothrow_swappable_v<std::vector<ValueType>> &&
        std::is_nothrow_swappable_v<std::vector<std::uint64_t>> &&
        std::is_nothrow_swappable_v<Hash> &&
        std::is_nothrow_swappable_v<KeyEqual>) {
        using std::swap;
        swap(values_, other.values_);
        swap(buckets_, other.buckets_);
        swap(max_size_, other.max_size_);
        swap(shift_, other.shift_);
        swap(hash_, other.hash_);
        swap(eq_, other.eq_);
    }

    const std::vector<ValueType>& values() const noexcept { return values_; }

    iterator begin() noexcept { return values_.begin(); }
    iterator end() noexcept { return values_.end(); }
    const_iterator begin() const noexcept { return values_.begin(); }
    const_iterator end() const noexcept { return values_.end(); }
    const_iterator cbegin() const noexcept { return values_.cbegin(); }
    const_iterator cend() const noexcept { return values_.cend(); }

    const Hash& hash_function() const noexcept { return hash_; }
    const KeyEqual& key_eq() const noexcept { return eq_; }

private:
    static constexpr std::size_t MIN_BUCKETS = 8;
    // 上位 24 bit: 探索距離 + 1 (0 は空きバケット)、中位 8 bit: フィンガープリント、下位 32 bit: 密配列添字
    static constexpr std::uint64_t DIST_INC = 1ULL << 40;
    static constexpr std::uint64_t UPPER_MASK = ~0xFFFFFFFFULL;
    static constexpr std::uint64_t IDX_MASK = 0xFFFFFFFFULL;

    std::vector<ValueType> values_;
    std::vector<std::uint64_t> buckets_;
    std::size_t max_size_ = 0;
    std::uint32_t shift_ = 0;
    [[no_unique_address]] Hash hash_{};
    [[no_unique_address]] KeyEqual eq_{};

    template <typename K>
    static constexpr bool is_nothrow_hash_key() noexcept {
        if constexpr (transparent_string_key<Key, Hash, KeyEqual, K>) {
            return noexcept(std::declval<const Hash&>()(
                std::string_view(std::declval<const K&>())));
        } else {
            return noexcept(std::declval<const Hash&>()(std::declval<const K&>()));
        }
    }

    template <typename K>
    static constexpr bool is_nothrow_eq_key() noexcept {
        if constexpr (transparent_string_key<Key, Hash, KeyEqual, K>) {
            return true;
        } else {
            return noexcept(std::declval<const KeyEqual&>()(
                std::declval<const Key&>(), std::declval<const K&>()));
        }
    }

    template <typename K>
    std::uint64_t hash_key(const K& key) const noexcept(is_nothrow_hash_key<K>()) {
        if constexpr (transparent_string_key<Key, Hash, KeyEqual, K>) {
            return hash_(std::string_view(key));
        } else {
            const std::uint64_t h = static_cast<std::uint64_t>(hash_(key));
            if constexpr (has_is_avalanching<Hash>) {
                return h;
            } else {
                return splitmix64(h ^ hash_seed());
            }
        }
    }

    template <typename K>
    bool eq_key(const Key& a, const K& b) const noexcept(is_nothrow_eq_key<K>()) {
        if constexpr (transparent_string_key<Key, Hash, KeyEqual, K>) {
            return std::string_view(a) == std::string_view(b);
        } else {
            return static_cast<bool>(eq_(a, b));
        }
    }

    static constexpr std::uint64_t make_dist_fp(std::uint64_t h) noexcept {
        return DIST_INC | ((h & 0xFFULL) << 32);
    }

    static std::size_t calc_buckets(std::size_t n) noexcept {
        // 最大負荷率 75% (3/4) を維持する最小の 2 のべき乗バケット数を求める
        const std::size_t need =
            (n <= std::numeric_limits<std::size_t>::max() / 4)
                ? ((n * 4 + 2) / 3)
                : n;
        return std::bit_ceil(std::max<std::size_t>(MIN_BUCKETS, need));
    }

    template <typename... Args>
    std::pair<iterator, bool> emplace_fresh_with_hash(std::uint64_t h, Args&&... args) {
        std::uint64_t dist_fp = make_dist_fp(h);
        std::size_t pos = h >> shift_;
        const std::size_t mask = buckets_.size() - 1;
        while (dist_fp <= buckets_[pos]) {
            dist_fp += DIST_INC;
            pos = (pos + 1) & mask;
        }
        const std::uint32_t new_idx = static_cast<std::uint32_t>(values_.size());
        assert(values_.size() < (1ULL << 32));
        values_.emplace_back(std::forward<Args>(args)...);
        place_and_shift(pos, dist_fp | new_idx);
        return {values_.begin() + static_cast<std::ptrdiff_t>(new_idx), true};
    }

    void place_and_shift(std::size_t pos, std::uint64_t entry) noexcept {
        const std::size_t mask = buckets_.size() - 1;
        while (buckets_[pos] != 0) {
            if ((entry & UPPER_MASK) > (buckets_[pos] & UPPER_MASK)) {
                std::swap(entry, buckets_[pos]);
            }
            entry += DIST_INC;
            pos = (pos + 1) & mask;
        }
        buckets_[pos] = entry;
    }

    std::size_t find_bucket_by_idx(std::uint32_t target_idx, const Key& key) const
        noexcept(noexcept(hash_key(key))) {
        const std::uint64_t h = hash_key(key);
        std::size_t pos = h >> shift_;
        const std::size_t mask = buckets_.size() - 1;
        std::uint64_t dist_fp = make_dist_fp(h);
        while (true) {
            if (buckets_[pos] == (dist_fp | target_idx)) {
                return pos;
            }
            dist_fp += DIST_INC;
            pos = (pos + 1) & mask;
        }
    }

    void shift_back_bucket(std::size_t pos) noexcept {
        const std::size_t mask = buckets_.size() - 1;
        std::size_t next_pos = (pos + 1) & mask;
        while (buckets_[next_pos] >= 2 * DIST_INC) {
            buckets_[pos] = buckets_[next_pos] - DIST_INC;
            pos = next_pos;
            next_pos = (pos + 1) & mask;
        }
        buckets_[pos] = 0;
    }

    void erase_bucket_only(std::uint32_t val_idx) noexcept {
        shift_back_bucket(find_bucket_by_idx(val_idx, KeyOfValue{}(values_[val_idx])));
    }

    void erase_at(std::size_t pos, std::uint32_t val_idx) {
        shift_back_bucket(pos);
        const std::uint32_t last_idx = static_cast<std::uint32_t>(values_.size() - 1);
        if (val_idx != last_idx) {
            values_[val_idx] = std::move(values_[last_idx]);
            const std::size_t p =
                find_bucket_by_idx(last_idx, KeyOfValue{}(values_[val_idx]));
            buckets_[p] = (buckets_[p] & UPPER_MASK) | val_idx;
        }
        values_.pop_back();
    }

    void rehash_impl(std::size_t new_cap) {
        buckets_.assign(new_cap, 0ULL);
        shift_ = static_cast<std::uint32_t>(64 - std::countr_zero(new_cap));
        max_size_ = new_cap - (new_cap >> 2);
        values_.reserve(max_size_);
        for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(values_.size()); ++i) {
            const std::uint64_t h = hash_key(KeyOfValue{}(values_[i]));
            const std::uint64_t dist_fp = make_dist_fp(h);
            const std::size_t pos = h >> shift_;
            place_and_shift(pos, dist_fp | i);
        }
    }
};

}  // namespace hash_detail
