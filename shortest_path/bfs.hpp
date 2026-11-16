#pragma once

#include <cassert>
#include <queue>
#include <utility>
#include <vector>

#include "common.hpp"

namespace shortest_path_detail {

template <bool TrackParent>
auto bfs_impl(const std::vector<std::vector<long long>>& g,
              const std::vector<long long>& starts,
              long long inf) {
    const long long n = static_cast<long long>(g.size());
    std::vector<long long> dist(n, inf);
    std::vector<long long> parent;
    if constexpr (TrackParent) {
        parent.assign(n, -1);
    }

    std::queue<long long> que;
    for (long long s : starts) {
        assert(0 <= s && s < n);
        if (dist[s] == 0) continue;
        dist[s] = 0;
        que.push(s);
    }

    while (!que.empty()) {
        const long long v = que.front();
        que.pop();
        for (long long to : g[v]) {
            assert(0 <= to && to < n);
            if (dist[to] == inf) {
                dist[to] = dist[v] + 1;
                if constexpr (TrackParent) {
                    parent[to] = v;
                }
                que.push(to);
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
 * @brief 重みなしグラフの多始点 BFS で最短距離を求める
 * @param g 隣接リスト (0-indexed, 頂点数 N)
 * @param starts 始点の頂点列 (0-indexed)
 * @param inf 到達不可能を表す値 (デフォルト: 4e18)
 * @return 各頂点への最短距離配列 (到達不可能な頂点は inf)
 * @note 時間計算量: O(N + M), 空間計算量: O(N)
 */
inline std::vector<long long> bfs(
    const std::vector<std::vector<long long>>& g,
    const std::vector<long long>& starts,
    long long inf = shortest_path_detail::default_inf<long long>()) {
    return shortest_path_detail::bfs_impl<false>(g, starts, inf);
}

/**
 * @brief 重みなしグラフの単始点 BFS で最短距離を求める
 * @param g 隣接リスト (0-indexed, 頂点数 N)
 * @param start 始点 (0-indexed)
 * @param inf 到達不可能を表す値 (デフォルト: 4e18)
 * @return 各頂点への最短距離配列 (到達不可能な頂点は inf)
 * @note 時間計算量: O(N + M), 空間計算量: O(N)
 */
inline std::vector<long long> bfs(
    const std::vector<std::vector<long long>>& g,
    long long start,
    long long inf = shortest_path_detail::default_inf<long long>()) {
    return bfs(g, std::vector<long long>{start}, inf);
}

/**
 * @brief 重みなしグラフの多始点 BFS で最短距離と親頂点配列を求める
 * @param g 隣接リスト (0-indexed, 頂点数 N)
 * @param starts 始点の頂点列 (0-indexed)
 * @param inf 到達不可能を表す値 (デフォルト: 4e18)
 * @return {dist, parent} (到達不可能な頂点の dist は inf、始点および未到達頂点の parent は -1)
 * @note 時間計算量: O(N + M), 空間計算量: O(N)
 */
inline std::pair<std::vector<long long>, std::vector<long long>> bfs_with_parent(
    const std::vector<std::vector<long long>>& g,
    const std::vector<long long>& starts,
    long long inf = shortest_path_detail::default_inf<long long>()) {
    return shortest_path_detail::bfs_impl<true>(g, starts, inf);
}

/**
 * @brief 重みなしグラフの単始点 BFS で最短距離と親頂点配列を求める
 * @param g 隣接リスト (0-indexed, 頂点数 N)
 * @param start 始点 (0-indexed)
 * @param inf 到達不可能を表す値 (デフォルト: 4e18)
 * @return {dist, parent} (到達不可能な頂点の dist は inf、始点および未到達頂点の parent は -1)
 * @note 時間計算量: O(N + M), 空間計算量: O(N)
 */
inline std::pair<std::vector<long long>, std::vector<long long>> bfs_with_parent(
    const std::vector<std::vector<long long>>& g,
    long long start,
    long long inf = shortest_path_detail::default_inf<long long>()) {
    return bfs_with_parent(g, std::vector<long long>{start}, inf);
}
