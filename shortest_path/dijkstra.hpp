#pragma once

#include <cassert>
#include <functional>
#include <queue>
#include <type_traits>
#include <utility>
#include <vector>

#include "common.hpp"

namespace shortest_path_detail {

template <bool TrackParent, typename T>
auto dijkstra_impl(const std::vector<std::vector<std::pair<long long, T>>>& g,
                   const std::vector<long long>& starts,
                   T inf) {
    const long long n = static_cast<long long>(g.size());
    std::vector<T> dist(n, inf);
    std::vector<long long> parent;
    if constexpr (TrackParent) {
        parent.assign(n, -1);
    }

    using P = std::pair<T, long long>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

    for (long long s : starts) {
        assert(0 <= s && s < n);
        if (dist[s] == T{0}) continue;
        dist[s] = T{0};
        pq.emplace(T{0}, s);
    }

    while (!pq.empty()) {
        const auto [d, v] = pq.top();
        pq.pop();
        if (d > dist[v]) continue;

        for (const auto& [to, w] : g[v]) {
            assert(0 <= to && to < n);
            assert(w >= T{0});
            const T nd = d + w;
            if (nd < dist[to]) {
                dist[to] = nd;
                if constexpr (TrackParent) {
                    parent[to] = v;
                }
                pq.emplace(nd, to);
            }
        }
    }

    if constexpr (TrackParent) {
        return std::make_pair(std::move(dist), std::move(parent));
    } else {
        return dist;
    }
}

}  // namespace shortest_path_detail

/**
 * @brief 非負重み付きグラフの多始点ダイクストラ法で最短距離を求める
 * @tparam T 辺の重み・距離の型 (デフォルト: long long)
 * @param g 隣接リスト g[u] = {{v, weight}, ...} (0-indexed, 頂点数 N, weight >= 0)
 * @param starts 始点の頂点列 (0-indexed)
 * @param inf 到達不可能を表す値 (デフォルト: long long の場合 4e18)
 * @return 各頂点への最短距離配列 (到達不可能な頂点は inf)
 * @note 前提: すべての辺の重みが非負であること。時間計算量: O((N + M) log N), 空間計算量: O(N + M)
 */
template <typename T = long long>
std::vector<T> dijkstra(
    const std::vector<std::vector<std::pair<long long, T>>>& g,
    const std::vector<long long>& starts,
    std::type_identity_t<T> inf = shortest_path_detail::default_inf<T>()) {
    return shortest_path_detail::dijkstra_impl<false>(g, starts, inf);
}

/**
 * @brief 非負重み付きグラフの単始点ダイクストラ法で最短距離を求める
 * @tparam T 辺の重み・距離の型 (デフォルト: long long)
 * @param g 隣接リスト g[u] = {{v, weight}, ...} (0-indexed, 頂点数 N, weight >= 0)
 * @param start 始点 (0-indexed)
 * @param inf 到達不可能を表す値 (デフォルト: long long の場合 4e18)
 * @return 各頂点への最短距離配列 (到達不可能な頂点は inf)
 * @note 前提: すべての辺の重みが非負であること。時間計算量: O((N + M) log N), 空間計算量: O(N + M)
 */
template <typename T = long long>
std::vector<T> dijkstra(
    const std::vector<std::vector<std::pair<long long, T>>>& g,
    long long start,
    std::type_identity_t<T> inf = shortest_path_detail::default_inf<T>()) {
    return dijkstra(g, std::vector<long long>{start}, inf);
}

/**
 * @brief 非負重み付きグラフの多始点ダイクストラ法で最短距離と親頂点配列を求める
 * @tparam T 辺の重み・距離の型 (デフォルト: long long)
 * @param g 隣接リスト g[u] = {{v, weight}, ...} (0-indexed, 頂点数 N, weight >= 0)
 * @param starts 始点の頂点列 (0-indexed)
 * @param inf 到達不可能を表す値 (デフォルト: long long の場合 4e18)
 * @return {dist, parent} (到達不可能な頂点の dist は inf、始点および未到達頂点の parent は -1)
 * @note 前提: すべての辺の重みが非負であること。時間計算量: O((N + M) log N), 空間計算量: O(N + M)
 */
template <typename T = long long>
std::pair<std::vector<T>, std::vector<long long>> dijkstra_with_parent(
    const std::vector<std::vector<std::pair<long long, T>>>& g,
    const std::vector<long long>& starts,
    std::type_identity_t<T> inf = shortest_path_detail::default_inf<T>()) {
    return shortest_path_detail::dijkstra_impl<true>(g, starts, inf);
}

/**
 * @brief 非負重み付きグラフの単始点ダイクストラ法で最短距離と親頂点配列を求める
 * @tparam T 辺の重み・距離の型 (デフォルト: long long)
 * @param g 隣接リスト g[u] = {{v, weight}, ...} (0-indexed, 頂点数 N, weight >= 0)
 * @param start 始点 (0-indexed)
 * @param inf 到達不可能を表す値 (デフォルト: long long の場合 4e18)
 * @return {dist, parent} (到達不可能な頂点の dist は inf、始点および未到達頂点の parent は -1)
 * @note 前提: すべての辺の重みが非負であること。時間計算量: O((N + M) log N), 空間計算量: O(N + M)
 */
template <typename T = long long>
std::pair<std::vector<T>, std::vector<long long>> dijkstra_with_parent(
    const std::vector<std::vector<std::pair<long long, T>>>& g,
    long long start,
    std::type_identity_t<T> inf = shortest_path_detail::default_inf<T>()) {
    return dijkstra_with_parent(g, std::vector<long long>{start}, inf);
}
