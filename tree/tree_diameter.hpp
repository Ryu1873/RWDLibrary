#pragma once

#include <algorithm>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>

namespace tree_detail {

inline std::vector<long long> reconstruct_path(
    const std::vector<long long>& parent, long long end) {
    std::vector<long long> path;
    for (long long cur = end; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

template <bool TrackParent>
auto diameter_bfs_unweighted(const std::vector<std::vector<long long>>& g,
                             long long start) {
    const long long n = static_cast<long long>(g.size());
    std::vector<long long> dist(n, -1);
    std::vector<long long> parent;
    if constexpr (TrackParent) {
        parent.assign(n, -1);
    }
    std::queue<long long> que;

    dist[start] = 0;
    que.push(start);

    long long farthest = start;
    while (!que.empty()) {
        const long long v = que.front();
        que.pop();
        if (dist[v] > dist[farthest]) {
            farthest = v;
        }
        for (long long to : g[v]) {
            if (dist[to] == -1) {
                dist[to] = dist[v] + 1;
                if constexpr (TrackParent) {
                    parent[to] = v;
                }
                que.push(to);
            }
        }
    }
    return std::make_tuple(farthest, dist[farthest], std::move(parent));
}

template <bool TrackParent, typename T>
auto diameter_bfs_weighted(
    const std::vector<std::vector<std::pair<long long, T>>>& g,
    long long start) {
    const long long n = static_cast<long long>(g.size());
    std::vector<T> dist(n, T{0});
    std::vector<long long> parent;
    if constexpr (TrackParent) {
        parent.assign(n, -1);
    }
    std::vector<char> visited(n, 0);
    std::queue<long long> que;

    visited[start] = 1;
    que.push(start);

    long long farthest = start;
    while (!que.empty()) {
        const long long v = que.front();
        que.pop();
        if (dist[v] > dist[farthest]) {
            farthest = v;
        }
        for (const auto& [to, w] : g[v]) {
            if (!visited[to]) {
                visited[to] = 1;
                dist[to] = dist[v] + w;
                if constexpr (TrackParent) {
                    parent[to] = v;
                }
                que.push(to);
            }
        }
    }
    return std::make_tuple(farthest, dist[farthest], std::move(parent));
}

}  // namespace tree_detail

/**
 * @brief 重みなし木の直径（最長パスの辺数）を返す
 * @param g 隣接リスト (0-indexed, 頂点数 N)
 * @return 木の直径 (N == 0 の場合は 0)
 * @note 時間計算量: O(N), 空間計算量: O(N)
 */
inline long long tree_diameter(const std::vector<std::vector<long long>>& g) {
    if (g.empty()) return 0;
    const auto [u, _d1, _p1] = tree_detail::diameter_bfs_unweighted<false>(g, 0);
    const auto [_v, diameter, _p2] =
        tree_detail::diameter_bfs_unweighted<false>(g, u);
    return diameter;
}

/**
 * @brief 重みなし木の直径となるパス上の頂点列を返す
 * @param g 隣接リスト (0-indexed, 頂点数 N)
 * @return 直径の端点 u からもう一方の端点 v までの頂点列 (N == 0 の場合は空)
 * @note 時間計算量: O(N), 空間計算量: O(N)
 */
inline std::vector<long long> tree_diameter_path(
    const std::vector<std::vector<long long>>& g) {
    if (g.empty()) return {};
    const auto [u, _d1, _p1] = tree_detail::diameter_bfs_unweighted<false>(g, 0);
    const auto [v, _d2, parent] =
        tree_detail::diameter_bfs_unweighted<true>(g, u);
    return tree_detail::reconstruct_path(parent, v);
}

/**
 * @brief 非負重み付き木の直径（最長パスの重み総和）を返す
 * @tparam T 辺の重みの型 (int, long long 等)
 * @param g 隣接リスト g[u] = {{v, weight}, ...} (0-indexed, 頂点数 N)
 * @return 木の直径 (N == 0 の場合は T{0})
 * @note 前提: すべての辺の重みが非負であること。時間計算量: O(N), 空間計算量: O(N)
 */
template <typename T>
T tree_diameter(const std::vector<std::vector<std::pair<long long, T>>>& g) {
    if (g.empty()) return T{0};
    const auto [u, _d1, _p1] = tree_detail::diameter_bfs_weighted<false>(g, 0);
    const auto [_v, diameter, _p2] =
        tree_detail::diameter_bfs_weighted<false>(g, u);
    return diameter;
}

/**
 * @brief 非負重み付き木の直径となるパス上の頂点列を返す
 * @tparam T 辺の重みの型 (int, long long 等)
 * @param g 隣接リスト g[u] = {{v, weight}, ...} (0-indexed, 頂点数 N)
 * @return 直径の端点 u からもう一方の端点 v までの頂点列 (N == 0 の場合は空)
 * @note 前提: すべての辺の重みが非負であること。時間計算量: O(N), 空間計算量: O(N)
 */
template <typename T>
std::vector<long long> tree_diameter_path(
    const std::vector<std::vector<std::pair<long long, T>>>& g) {
    if (g.empty()) return {};
    const auto [u, _d1, _p1] = tree_detail::diameter_bfs_weighted<false>(g, 0);
    const auto [v, _d2, parent] =
        tree_detail::diameter_bfs_weighted<true>(g, u);
    return tree_detail::reconstruct_path(parent, v);
}
