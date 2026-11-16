#pragma once

#include <cassert>
#include <deque>
#include <type_traits>
#include <utility>
#include <vector>

#include "common.hpp"

namespace shortest_path_detail {

template <bool TrackParent, typename T>
auto bfs01_impl(const std::vector<std::vector<std::pair<long long, T>>>& g,
                const std::vector<long long>& starts,
                T inf) {
    const long long n = static_cast<long long>(g.size());
    std::vector<T> dist(n, inf);
    std::vector<long long> parent;
    if constexpr (TrackParent) {
        parent.assign(n, -1);
    }
    std::vector<char> settled(n, 0);
    std::deque<long long> deq;

    for (long long s : starts) {
        assert(0 <= s && s < n);
        if (dist[s] == T{0}) continue;
        dist[s] = T{0};
        deq.push_back(s);
    }

    while (!deq.empty()) {
        const long long v = deq.front();
        deq.pop_front();
        if (settled[v]) continue;
        settled[v] = 1;

        for (const auto& [to, w] : g[v]) {
            assert(0 <= to && to < n);
            assert(w == T{0} || w == T{1});
            const T nd = dist[v] + w;
            if (nd < dist[to]) {
                dist[to] = nd;
                if constexpr (TrackParent) {
                    parent[to] = v;
                }
                if (w == T{0}) {
                    deq.push_front(to);
                } else {
                    deq.push_back(to);
                }
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
 * @brief 辺重みが 0 または 1 のグラフの多始点 01-BFS で最短距離を求める
 * @tparam T 辺の重み・距離の型 (デフォルト: long long)
 * @param g 隣接リスト g[u] = {{v, weight}, ...} (0-indexed, 頂点数 N, weight は 0 または 1)
 * @param starts 始点の頂点列 (0-indexed)
 * @param inf 到達不可能を表す値 (デフォルト: long long の場合 4e18)
 * @return 各頂点への最短距離配列 (到達不可能な頂点は inf)
 * @note 前提: すべての辺の重みが 0 または 1 であること。時間計算量: O(N + M), 空間計算量: O(N)
 */
template <typename T = long long>
std::vector<T> bfs01(
    const std::vector<std::vector<std::pair<long long, T>>>& g,
    const std::vector<long long>& starts,
    std::type_identity_t<T> inf = shortest_path_detail::default_inf<T>()) {
    return shortest_path_detail::bfs01_impl<false>(g, starts, inf);
}

/**
 * @brief 辺重みが 0 または 1 のグラフの単始点 01-BFS で最短距離を求める
 * @tparam T 辺の重み・距離の型 (デフォルト: long long)
 * @param g 隣接リスト g[u] = {{v, weight}, ...} (0-indexed, 頂点数 N, weight は 0 または 1)
 * @param start 始点 (0-indexed, デフォルト: 0)
 * @param inf 到達不可能を表す値 (デフォルト: long long の場合 4e18)
 * @return 各頂点への最短距離配列 (到達不可能な頂点は inf)
 * @note 前提: すべての辺の重みが 0 または 1 であること。時間計算量: O(N + M), 空間計算量: O(N)
 */
template <typename T = long long>
std::vector<T> bfs01(
    const std::vector<std::vector<std::pair<long long, T>>>& g,
    long long start = 0,
    std::type_identity_t<T> inf = shortest_path_detail::default_inf<T>()) {
    return bfs01(g, std::vector<long long>{start}, inf);
}

/**
 * @brief 辺重みが 0 または 1 のグラフの多始点 01-BFS で最短距離と親頂点配列を求める
 * @tparam T 辺の重み・距離の型 (デフォルト: long long)
 * @param g 隣接リスト g[u] = {{v, weight}, ...} (0-indexed, 頂点数 N, weight は 0 または 1)
 * @param starts 始点の頂点列 (0-indexed)
 * @param inf 到達不可能を表す値 (デフォルト: long long の場合 4e18)
 * @return {dist, parent} (到達不可能な頂点の dist は inf、始点および未到達頂点の parent は -1)
 * @note 前提: すべての辺の重みが 0 または 1 であること。時間計算量: O(N + M), 空間計算量: O(N)
 */
template <typename T = long long>
std::pair<std::vector<T>, std::vector<long long>> bfs01_with_parent(
    const std::vector<std::vector<std::pair<long long, T>>>& g,
    const std::vector<long long>& starts,
    std::type_identity_t<T> inf = shortest_path_detail::default_inf<T>()) {
    return shortest_path_detail::bfs01_impl<true>(g, starts, inf);
}

/**
 * @brief 辺重みが 0 または 1 のグラフの単始点 01-BFS で最短距離と親頂点配列を求める
 * @tparam T 辺の重み・距離の型 (デフォルト: long long)
 * @param g 隣接リスト g[u] = {{v, weight}, ...} (0-indexed, 頂点数 N, weight は 0 または 1)
 * @param start 始点 (0-indexed, デフォルト: 0)
 * @param inf 到達不可能を表す値 (デフォルト: long long の場合 4e18)
 * @return {dist, parent} (到達不可能な頂点の dist は inf、始点および未到達頂点の parent は -1)
 * @note 前提: すべての辺の重みが 0 または 1 であること。時間計算量: O(N + M), 空間計算量: O(N)
 */
template <typename T = long long>
std::pair<std::vector<T>, std::vector<long long>> bfs01_with_parent(
    const std::vector<std::vector<std::pair<long long, T>>>& g,
    long long start = 0,
    std::type_identity_t<T> inf = shortest_path_detail::default_inf<T>()) {
    return bfs01_with_parent(g, std::vector<long long>{start}, inf);
}
