#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <type_traits>
#include <vector>

namespace shortest_path_detail {

template <typename T>
constexpr T default_inf() {
    if constexpr (std::is_same_v<T, long long>) {
        return 4'000'000'000'000'000'000LL;
    } else {
        return std::numeric_limits<T>::max() / 4;
    }
}

}  // namespace shortest_path_detail

/**
 * @brief 親頂点配列から goal までの最短経路を復元する
 * @param parent 各頂点の親頂点 (始点および到達不可能な頂点は -1)
 * @param goal 目的地の頂点 (0-indexed)
 * @return 始点から goal までの頂点列 (parent を辿れる範囲)
 * @note goal が到達不可能な場合でも {goal} が返るため、到達可能性の判定も行う場合は
 *       dist 配列を受け取るオーバーロードを使用すること。
 *       時間計算量: O(経路長)
 */
inline std::vector<long long> restore_path(
    const std::vector<long long>& parent, long long goal) {
    const long long n = static_cast<long long>(parent.size());
    assert(0 <= goal && goal < n);
    std::vector<long long> path;
    for (long long cur = goal; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

/**
 * @brief 距離配列と親頂点配列から goal までの最短経路を復元する
 * @tparam T 距離の型
 * @param dist 各頂点への最短距離配列
 * @param parent 各頂点の親頂点 (始点および到達不可能な頂点は -1)
 * @param goal 目的地の頂点 (0-indexed)
 * @param inf 到達不可能を表す値
 * @return 始点から goal までの頂点列 (到達不可能な場合は空)
 * @note 時間計算量: O(経路長)
 */
template <typename T>
std::vector<long long> restore_path(
    const std::vector<T>& dist,
    const std::vector<long long>& parent,
    long long goal,
    std::type_identity_t<T> inf = shortest_path_detail::default_inf<T>()) {
    const long long n = static_cast<long long>(parent.size());
    assert(static_cast<long long>(dist.size()) == n);
    assert(0 <= goal && goal < n);
    if (dist[goal] == inf) return {};
    return restore_path(parent, goal);
}
