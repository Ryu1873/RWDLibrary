#pragma once

#include <algorithm>
#include <queue>
#include <utility>
#include <vector>


/**
 * @brief 重みなし木の直径となるパス上の頂点列を返す
 * @param g 隣接リスト (0-indexed, 頂点数 N)
 * @return 直径の端点 u からもう一方の端点 v までの頂点列 (N == 0 の場合は空)
 * @note 時間計算量: O(N), 空間計算量: O(N)
 */
inline std::vector<long long> tree_diameter_path(
    const std::vector<std::vector<long long>>& g) {
    const long long n = static_cast<long long>(g.size());
    if (n == 0) return {};

    auto bfs = [&](long long start) {
        std::vector<long long> dist(n, -1);
        std::vector<long long> parent(n, -1);
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
                    parent[to] = v;
                    que.push(to);
                }
            }
        }
        return std::make_pair(farthest, std::move(parent));
    };

    const auto [u, _] = bfs(0);
    const auto [v, parent] = bfs(u);

    std::vector<long long> path;
    for (long long cur = v; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
    return path;
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
    const long long n = static_cast<long long>(g.size());
    if (n == 0) return {};

    auto bfs = [&](long long start) {
        std::vector<T> dist(n, T{0});
        std::vector<long long> parent(n, -1);
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
                    parent[to] = v;
                    que.push(to);
                }
            }
        }
        return std::make_pair(farthest, std::move(parent));
    };

    const auto [u, _] = bfs(0);
    const auto [v, parent] = bfs(u);

    std::vector<long long> path;
    for (long long cur = v; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
    return path;
}
