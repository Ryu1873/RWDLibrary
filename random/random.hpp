#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

/**
 * @brief ランダムテスト用の乱数生成器・基本データ生成クラス
 */
class Random {
public:
    /**
     * @brief 非決定的シードで乱数生成器を初期化する
     */
    Random() : eng_(std::random_device{}()) {}

    /**
     * @brief 指定したシード値で乱数生成器を初期化する
     * @param seed 乱数シード
     */
    explicit Random(unsigned long long seed) : eng_(seed) {}

    /**
     * @brief 内部の std::mt19937_64 エンジンへの参照を返す
     */
    std::mt19937_64& engine() { return eng_; }

    /**
     * @brief [l, r] の範囲の一様整数乱数を返す
     * @param l 下限 (含む)
     * @param r 上限 (含む)
     */
    long long randint(long long l, long long r) {
        assert(l <= r);
        std::uniform_int_distribution<long long> dist(l, r);
        return dist(eng_);
    }

    /**
     * @brief [l, r) の範囲の一様整数乱数を返す
     * @param l 下限 (含む)
     * @param r 上限 (含まない)
     */
    long long randrange(long long l, long long r) {
        assert(l < r);
        return randint(l, r - 1);
    }

    /**
     * @brief [0, n) の範囲の一様整数乱数を返す
     * @param n 上限 (含まない, n > 0)
     */
    long long randrange(long long n) {
        assert(n > 0);
        return randint(0, n - 1);
    }

    /**
     * @brief [l, r) の範囲の一様実数乱数を返す
     * @param l 下限 (含む, デフォルト: 0.0)
     * @param r 上限 (含まない, デフォルト: 1.0)
     */
    double randreal(double l = 0.0, double r = 1.0) {
        assert(l <= r);
        if (l == r) return l;
        std::uniform_real_distribution<double> dist(l, r);
        return dist(eng_);
    }

    /**
     * @brief 確率 p で true を返す
     * @param p 成功確率 (0.0 <= p <= 1.0, デフォルト: 0.5)
     */
    bool bernoulli(double p = 0.5) {
        assert(0.0 <= p && p <= 1.0);
        std::bernoulli_distribution dist(p);
        return dist(eng_);
    }

    /**
     * @brief vector からランダムに 1 要素を選んで返す
     */
    template <typename T>
    const T& choice(const std::vector<T>& v) {
        assert(!v.empty());
        return v[static_cast<std::size_t>(randrange(static_cast<long long>(v.size())))];
    }

    /**
     * @brief vector をインプレースでランダムにシャッフルする
     */
    template <typename T>
    void shuffle(std::vector<T>& v) {
        std::shuffle(v.begin(), v.end(), eng_);
    }

    /**
     * @brief [base, base + n - 1] の長さ n のランダム順列を返す
     * @param n 順列の長さ
     * @param base 最小値 (デフォルト: 0)
     */
    std::vector<long long> perm(long long n, long long base = 0) {
        assert(n >= 0);
        std::vector<long long> res(static_cast<std::size_t>(n));
        std::iota(res.begin(), res.end(), base);
        shuffle(res);
        return res;
    }

    /**
     * @brief 長さ n、各要素が [l, r] のランダム数列を返す
     */
    std::vector<long long> rand_vec(long long n, long long l, long long r) {
        assert(n >= 0 && l <= r);
        std::vector<long long> res(static_cast<std::size_t>(n));
        for (long long i = 0; i < n; ++i) {
            res[static_cast<std::size_t>(i)] = randint(l, r);
        }
        return res;
    }

    /**
     * @brief 長さ n、各要素が [l, r] で重複のないランダム数列を返す
     * @note Floyd のアルゴリズムにより時間計算量 O(n), 空間計算量 O(n)
     */
    std::vector<long long> rand_unique_vec(long long n, long long l, long long r) {
        assert(n >= 0 && l <= r);
        const unsigned long long width =
            static_cast<unsigned long long>(r) - static_cast<unsigned long long>(l) + 1ULL;
        assert(static_cast<unsigned long long>(n) <= width);
        if (n == 0) return {};

        std::unordered_set<unsigned long long> used;
        used.reserve(static_cast<std::size_t>(n) * 2);
        std::vector<long long> res;
        res.reserve(static_cast<std::size_t>(n));

        const unsigned long long start = width - static_cast<unsigned long long>(n);
        for (unsigned long long i = start; i < width; ++i) {
            std::uniform_int_distribution<unsigned long long> dist(0ULL, i);
            const unsigned long long t = dist(eng_);
            const unsigned long long chosen = used.contains(t) ? i : t;
            used.insert(chosen);
            res.push_back(static_cast<long long>(static_cast<unsigned long long>(l) + chosen));
        }
        shuffle(res);
        return res;
    }

    /**
     * @brief [l, r] 内のランダムな閉区間 [a, b] (l <= a <= b <= r) を一様に返す
     */
    std::pair<long long, long long> rand_closed_interval(long long l, long long r) {
        assert(l <= r);
        long long a = randint(l, r + 1);
        long long b = randint(l, r + 1);
        while (a == b) {
            b = randint(l, r + 1);
        }
        if (a > b) std::swap(a, b);
        return {a, b - 1};
    }

    /**
     * @brief [l, r] 内のランダムな半開区間 [a, b) を一様に返す
     * @param allow_empty true のとき a == b (空区間) を許容する (l <= a <= b <= r)
     */
    std::pair<long long, long long> rand_half_open_interval(
        long long l, long long r, bool allow_empty = false) {
        if (allow_empty) {
            return rand_closed_interval(l, r);
        }
        assert(l < r);
        auto [a, b] = rand_closed_interval(l, r - 1);
        return {a, b + 1};
    }

    /**
     * @brief 指定文字集合から長さ n のランダム文字列を生成する
     */
    std::string rand_string(
        long long n,
        const std::string& charset = "abcdefghijklmnopqrstuvwxyz") {
        assert(n >= 0 && !charset.empty());
        std::string s(static_cast<std::size_t>(n), ' ');
        const long long m = static_cast<long long>(charset.size());
        for (long long i = 0; i < n; ++i) {
            s[static_cast<std::size_t>(i)] = charset[static_cast<std::size_t>(randrange(m))];
        }
        return s;
    }

    /**
     * @brief 長さ n のランダム英小文字文字列を生成する
     */
    std::string rand_lower_string(long long n) {
        return rand_string(n, "abcdefghijklmnopqrstuvwxyz");
    }

    /**
     * @brief 長さ n のランダム英大文字文字列を生成する
     */
    std::string rand_upper_string(long long n) {
        return rand_string(n, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    }

    /**
     * @brief 長さ n のランダム 01 文字列を生成する
     */
    std::string rand_binary_string(long long n) {
        return rand_string(n, "01");
    }

    /**
     * @brief H x W のランダムグリッドを生成する
     */
    std::vector<std::string> rand_grid(
        long long h, long long w, const std::string& charset = ".#") {
        assert(h >= 0 && w >= 0 && !charset.empty());
        std::vector<std::string> grid(static_cast<std::size_t>(h));
        for (long long i = 0; i < h; ++i) {
            grid[static_cast<std::size_t>(i)] = rand_string(w, charset);
        }
        return grid;
    }

private:
    std::mt19937_64 eng_;
};

/**
 * @brief コンテナの要素を区切り文字付きで 1 行出力する
 */
template <typename Container>
void print_vec(const Container& v, std::ostream& os = std::cout, char sep = ' ') {
    bool first = true;
    for (const auto& x : v) {
        if (!first) os << sep;
        os << x;
        first = false;
    }
    os << '\n';
}

/**
 * @brief グリッドを各行出力する
 */
inline void print_grid(const std::vector<std::string>& grid, std::ostream& os = std::cout) {
    for (const auto& row : grid) {
        os << row << '\n';
    }
}
