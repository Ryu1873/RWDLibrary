#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

#include "random.hpp"

/**
 * @brief 重み付き辺を表す構造体
 * @tparam T 辺の重みの型 (デフォルト: long long)
 */
template <typename T = long long>
struct WeightedEdge {
    long long u;
    long long v;
    T w;
};

namespace random_detail {

inline unsigned long long encode_edge(long long u, long long v) {
    return (static_cast<unsigned long long>(u) << 32) |
           static_cast<unsigned long long>(v);
}

inline void randomize_edge_directions(
    Random& rng, std::vector<std::pair<long long, long long>>& edges) {
    for (auto& [u, v] : edges) {
        if (rng.bernoulli()) std::swap(u, v);
    }
}

inline std::vector<std::pair<long long, long long>> sample_distinct_pairs(
    Random& rng,
    long long n,
    long long m,
    bool directed,
    const std::vector<std::pair<long long, long long>>& initial_edges = {}) {
    assert(0 <= n && n <= (1LL << 30));
    const long long max_edges = directed ? n * (n - 1) : n * (n - 1) / 2;
    assert(static_cast<long long>(initial_edges.size()) <= m && m <= max_edges);
    if (m == 0) return {};

    std::unordered_set<unsigned long long> used;
    used.reserve(static_cast<std::size_t>(m) * 2);
    std::vector<std::pair<long long, long long>> edges;
    edges.reserve(static_cast<std::size_t>(m));

    for (auto [u, v] : initial_edges) {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v);
        if (!directed && u > v) std::swap(u, v);
        const auto key = encode_edge(u, v);
        assert(!used.contains(key));
        used.insert(key);
        edges.emplace_back(u, v);
    }

    if (m <= max_edges / 2) {
        while (static_cast<long long>(edges.size()) < m) {
            long long u = rng.randrange(n);
            long long v = rng.randrange(n);
            if (u == v) continue;
            if (!directed && u > v) std::swap(u, v);
            const auto key = encode_edge(u, v);
            if (used.insert(key).second) {
                edges.emplace_back(u, v);
            }
        }
    } else {
        std::vector<std::pair<long long, long long>> candidates;
        candidates.reserve(static_cast<std::size_t>(max_edges - static_cast<long long>(edges.size())));
        for (long long u = 0; u < n; ++u) {
            const long long v_start = directed ? 0 : u + 1;
            for (long long v = v_start; v < n; ++v) {
                if (u == v) continue;
                if (!used.contains(encode_edge(u, v))) {
                    candidates.emplace_back(u, v);
                }
            }
        }
        rng.shuffle(candidates);
        const long long need = m - static_cast<long long>(edges.size());
        for (long long i = 0; i < need; ++i) {
            edges.push_back(candidates[static_cast<std::size_t>(i)]);
        }
    }

    rng.shuffle(edges);
    return edges;
}

}  // namespace random_detail

/**
 * @brief N 頂点のラベル付き木を一様ランダムに生成する (Prüfer 列を使用)
 * @param rng Random インスタンス
 * @param n 頂点数 (0-indexed, n >= 0)
 * @return n - 1 本の辺リスト (n <= 1 の場合は空)
 * @note 時間計算量: O(n), 空間計算量: O(n)
 */
inline std::vector<std::pair<long long, long long>> random_tree(
    Random& rng, long long n) {
    assert(n >= 0);
    if (n <= 1) return {};
    if (n == 2) {
        std::vector<std::pair<long long, long long>> edges = {{0, 1}};
        random_detail::randomize_edge_directions(rng, edges);
        return edges;
    }

    const auto prufer = rng.rand_vec(n - 2, 0, n - 1);
    std::vector<long long> degree(static_cast<std::size_t>(n), 1);
    for (long long v : prufer) {
        degree[static_cast<std::size_t>(v)]++;
    }

    long long ptr = 0;
    while (degree[static_cast<std::size_t>(ptr)] != 1) ptr++;
    long long leaf = ptr;

    std::vector<std::pair<long long, long long>> edges;
    edges.reserve(static_cast<std::size_t>(n - 1));
    for (long long v : prufer) {
        edges.emplace_back(leaf, v);
        degree[static_cast<std::size_t>(leaf)]--;
        if (--degree[static_cast<std::size_t>(v)] == 1 && v < ptr) {
            leaf = v;
        } else {
            ptr++;
            while (degree[static_cast<std::size_t>(ptr)] != 1) ptr++;
            leaf = ptr;
        }
    }
    edges.emplace_back(leaf, n - 1);

    random_detail::randomize_edge_directions(rng, edges);
    rng.shuffle(edges);
    return edges;
}

/**
 * @brief N 頂点 M 辺の単純無向グラフを生成する
 * @note 前提: 0 <= m <= n * (n - 1) / 2
 */
inline std::vector<std::pair<long long, long long>> random_simple_graph(
    Random& rng, long long n, long long m) {
    auto edges = random_detail::sample_distinct_pairs(rng, n, m, false);
    random_detail::randomize_edge_directions(rng, edges);
    return edges;
}

/**
 * @brief N 頂点 M 辺の単純連結無向グラフを生成する
 * @note 前提: n == 0 なら m == 0、n >= 1 なら n - 1 <= m <= n * (n - 1) / 2
 */
inline std::vector<std::pair<long long, long long>> random_connected_graph(
    Random& rng, long long n, long long m) {
    if (n == 0) {
        assert(m == 0);
        return {};
    }
    assert(m >= n - 1);
    auto tree = random_tree(rng, n);
    auto edges = random_detail::sample_distinct_pairs(rng, n, m, false, tree);
    random_detail::randomize_edge_directions(rng, edges);
    return edges;
}

/**
 * @brief N 頂点 M 辺の単純有向グラフを生成する (自己ループ・重複辺なし)
 * @note 前提: 0 <= m <= n * (n - 1)
 */
inline std::vector<std::pair<long long, long long>> random_directed_graph(
    Random& rng, long long n, long long m) {
    return random_detail::sample_distinct_pairs(rng, n, m, true);
}

/**
 * @brief N 頂点 M 辺の単純 DAG (有向非巡回グラフ) を生成する
 * @note 前提: 0 <= m <= n * (n - 1) / 2
 */
inline std::vector<std::pair<long long, long long>> random_dag(
    Random& rng, long long n, long long m) {
    auto edges = random_detail::sample_distinct_pairs(rng, n, m, false);
    const auto order = rng.perm(n);
    for (auto& [u, v] : edges) {
        u = order[static_cast<std::size_t>(u)];
        v = order[static_cast<std::size_t>(v)];
    }
    return edges;
}

/**
 * @brief N 頂点 M 辺のマルチグラフ (多重辺許容) を生成する
 * @param directed true の場合は有向グラフ、false の場合は無向グラフ
 * @param allow_self_loop true の場合は自己ループを許容する
 */
inline std::vector<std::pair<long long, long long>> random_multigraph(
    Random& rng,
    long long n,
    long long m,
    bool directed = false,
    bool allow_self_loop = true) {
    assert(n >= 0 && m >= 0);
    if (n == 0) {
        assert(m == 0);
        return {};
    }
    if (!allow_self_loop && m > 0) {
        assert(n >= 2);
    }

    std::vector<std::pair<long long, long long>> edges;
    edges.reserve(static_cast<std::size_t>(m));
    for (long long i = 0; i < m; ++i) {
        long long u = rng.randrange(n);
        long long v = rng.randrange(n);
        while (!allow_self_loop && u == v) {
            v = rng.randrange(n);
        }
        if (!directed && rng.bernoulli()) std::swap(u, v);
        edges.emplace_back(u, v);
    }
    return edges;
}

/**
 * @brief 辺リストの各辺に [min_w, max_w] (整数型は閉区間、浮動小数点型は半開区間) の重みを付与する
 * @tparam T 重みの型 (デフォルト: long long)
 */
template <typename T = long long>
std::vector<WeightedEdge<T>> assign_weights(
    Random& rng,
    const std::vector<std::pair<long long, long long>>& edges,
    std::type_identity_t<T> min_w,
    std::type_identity_t<T> max_w) {
    assert(min_w <= max_w);
    std::vector<WeightedEdge<T>> res;
    res.reserve(edges.size());
    for (const auto& [u, v] : edges) {
        T w;
        if constexpr (std::is_integral_v<T>) {
            w = static_cast<T>(
                rng.randint(static_cast<long long>(min_w), static_cast<long long>(max_w)));
        } else {
            w = static_cast<T>(
                rng.randreal(static_cast<double>(min_w), static_cast<double>(max_w)));
        }
        res.push_back(WeightedEdge<T>{u, v, w});
    }
    return res;
}

/**
 * @brief 重みなし辺リストを隣接リストに変換する
 * @param n 頂点数
 * @param edges 辺リスト (0-indexed)
 * @param directed true の場合は有向グラフ、false の場合は無向グラフ
 */
inline std::vector<std::vector<long long>> to_adj_list(
    long long n,
    const std::vector<std::pair<long long, long long>>& edges,
    bool directed = false) {
    assert(n >= 0);
    std::vector<std::vector<long long>> g(static_cast<std::size_t>(n));
    for (const auto& [u, v] : edges) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        g[static_cast<std::size_t>(u)].push_back(v);
        if (!directed) {
            g[static_cast<std::size_t>(v)].push_back(u);
        }
    }
    return g;
}

/**
 * @brief 重み付き辺リストを隣接リスト g[u] = {{v, weight}, ...} に変換する
 * @tparam T 重みの型
 * @param n 頂点数
 * @param edges 重み付き辺リスト (0-indexed)
 * @param directed true の場合は有向グラフ、false の場合は無向グラフ
 */
template <typename T>
std::vector<std::vector<std::pair<long long, T>>> to_adj_list(
    long long n,
    const std::vector<WeightedEdge<T>>& edges,
    bool directed = false) {
    assert(n >= 0);
    std::vector<std::vector<std::pair<long long, T>>> g(static_cast<std::size_t>(n));
    for (const auto& [u, v, w] : edges) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        g[static_cast<std::size_t>(u)].emplace_back(v, w);
        if (!directed) {
            g[static_cast<std::size_t>(v)].emplace_back(u, w);
        }
    }
    return g;
}

/**
 * @brief 重みなし辺リストを標準出力などへ出力する
 * @param index_base 頂点番号のオフセット (デフォルト: 1)
 */
inline void print_edges(
    const std::vector<std::pair<long long, long long>>& edges,
    std::ostream& os = std::cout,
    long long index_base = 1) {
    for (const auto& [u, v] : edges) {
        os << (u + index_base) << ' ' << (v + index_base) << '\n';
    }
}

/**
 * @brief 重み付き辺リストを標準出力などへ出力する
 * @param index_base 頂点番号のオフセット (デフォルト: 1)
 */
template <typename T>
void print_edges(
    const std::vector<WeightedEdge<T>>& edges,
    std::ostream& os = std::cout,
    long long index_base = 1) {
    for (const auto& [u, v, w] : edges) {
        os << (u + index_base) << ' ' << (v + index_base) << ' ' << w << '\n';
    }
}

/**
 * @brief 重みなしグラフを "N M" に続けて辺リスト形式で出力する
 */
inline void print_graph(
    long long n,
    const std::vector<std::pair<long long, long long>>& edges,
    std::ostream& os = std::cout,
    long long index_base = 1) {
    os << n << ' ' << edges.size() << '\n';
    print_edges(edges, os, index_base);
}

/**
 * @brief 重み付きグラフを "N M" に続けて辺リスト形式で出力する
 */
template <typename T>
void print_graph(
    long long n,
    const std::vector<WeightedEdge<T>>& edges,
    std::ostream& os = std::cout,
    long long index_base = 1) {
    os << n << ' ' << edges.size() << '\n';
    print_edges(edges, os, index_base);
}

/**
 * @brief 重みなし木を "N" に続けて辺リスト形式で出力する
 */
inline void print_tree(
    long long n,
    const std::vector<std::pair<long long, long long>>& edges,
    std::ostream& os = std::cout,
    long long index_base = 1) {
    os << n << '\n';
    print_edges(edges, os, index_base);
}

/**
 * @brief 重み付き木を "N" に続けて辺リスト形式で出力する
 */
template <typename T>
void print_tree(
    long long n,
    const std::vector<WeightedEdge<T>>& edges,
    std::ostream& os = std::cout,
    long long index_base = 1) {
    os << n << '\n';
    print_edges(edges, os, index_base);
}
