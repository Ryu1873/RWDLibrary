# random 使い方ガイド

ランダムテスト（ストレステスト）用の乱数生成・テストケース出力ライブラリです。  
`template/test.sh` の `gen.cpp` で標準出力にテストケースを生成する用途と、C++ コード内で直接データを生成する用途の両方に対応しています。

## ファイル構成

| ファイル | 役割 |
| --- | --- |
| `random.hpp` | `Random` クラス（基本乱数・順列・数列・区間・文字列・グリッド生成）と出力ヘルパー |
| `random_graph.hpp` | 木・各種グラフ生成、重み付与、隣接リスト変換、辺・グラフ出力ヘルパー（内部で `random.hpp` をインクルード） |

> `random_graph.hpp` をインクルードすれば `random.hpp` も読み込まれます。

---

## クイックスタート

### 1. `gen.cpp` での標準出力生成（`template/test.sh` 向け）

#### 数列・クエリの生成例

```cpp
#include <iostream>
#include "random/random.hpp"

int main() {
    Random rng; // 毎回ランダムなシード（固定したい場合は Random rng(42);）

    long long n = rng.randint(1, 10);
    long long q = rng.randint(1, 10);
    std::cout << n << ' ' << q << '\n';

    auto a = rng.rand_vec(n, 1, 100);
    print_vec(a);

    for (long long i = 0; i < q; ++i) {
        auto [l, r] = rng.rand_closed_interval(1, n);
        std::cout << l << ' ' << r << '\n';
    }
}
```

#### 重み付き連結無向グラフの生成例

```cpp
#include <iostream>
#include "random/random_graph.hpp"

int main() {
    Random rng;

    long long n = rng.randint(2, 8);
    long long m = rng.randint(n - 1, n * (n - 1) / 2);

    auto edges = random_connected_graph(rng, n, m);
    auto wedges = assign_weights(rng, edges, 1LL, 100LL);

    // 1行目に "N M"、続けて "u v w" (1-indexed) を出力
    print_graph(n, wedges);
}
```

#### 木の生成例

```cpp
#include <iostream>
#include "random/random_graph.hpp"

int main() {
    Random rng;
    long long n = rng.randint(1, 10);
    auto edges = random_tree(rng, n);

    // 1行目に "N"、続けて "u v" (1-indexed) を出力
    print_tree(n, edges);
}
```

### 2. C++ 内での直接テスト（隣接リスト変換）

生成関数は 0-indexed の辺リストを返すため、`to_adj_list` でそのまま `shortest_path/` や `tree/` の関数へ渡せます。

```cpp
#include "random/random_graph.hpp"
#include "shortest_path/dijkstra.hpp"

int main() {
    Random rng(42);
    long long n = 10, m = 20;

    auto edges = random_connected_graph(rng, n, m);
    auto wedges = assign_weights(rng, edges, 0LL, 1000LL);
    auto g = to_adj_list(n, wedges, false); // 無向グラフの隣接リスト

    auto dist = dijkstra(g, 0);
}
```

---

## `random.hpp` リファレンス

### 初期化

- `Random()`
  `std::random_device` による非決定的シードで初期化します。
- `explicit Random(unsigned long long seed)`
  指定したシード値で初期化します（再現性が必要なテスト向け）。
- `std::mt19937_64& engine()`
  内部の `std::mt19937_64` エンジンへの参照を返します。

### 基本乱数

| メソッド | 説明 | 条件 |
| --- | --- | --- |
| `randint(l, r)` | `[l, r]` の一様整数乱数（`long long`） | `l <= r` |
| `randrange(l, r)` | `[l, r)` の一様整数乱数（`long long`） | `l < r` |
| `randrange(n)` | `[0, n)` の一様整数乱数（`long long`） | `n > 0` |
| `randreal(l = 0.0, r = 1.0)` | `[l, r)` の一様実数乱数（`double`） | `l <= r` |
| `bernoulli(p = 0.5)` | 確率 `p` で `true` を返す（`bool`） | `0.0 <= p <= 1.0` |

### コンテナ操作・数列・順列

| メソッド | 説明 | 条件 |
| --- | --- | --- |
| `choice(v)` | `std::vector<T>` からランダムに 1 要素の参照を返す | `!v.empty()` |
| `shuffle(v)` | `std::vector<T>` をインプレースでシャッフルする | - |
| `perm(n, base = 0)` | `[base, base + n - 1]` のランダム順列（`std::vector<long long>`） | `n >= 0` |
| `rand_vec(n, l, r)` | 長さ `n`、各要素 `[l, r]` のランダム数列 | `n >= 0 && l <= r` |
| `rand_unique_vec(n, l, r)` | 長さ `n`、各要素 `[l, r]` で重複のないランダム数列（`O(n)`） | `n >= 0 && l <= r` かつ `n <= r - l + 1` |

### 区間生成

| メソッド | 説明 | 条件 |
| --- | --- | --- |
| `rand_closed_interval(l, r)` | `[l, r]` 内の閉区間 `[a, b]`（`l <= a <= b <= r`）を一様に返す | `l <= r` |
| `rand_half_open_interval(l, r, allow_empty = false)` | 半開区間 `[a, b)` を一様に返す。`allow_empty == false` なら `l <= a < b <= r`、`true` なら `l <= a <= b <= r` | `allow_empty ? l <= r : l < r` |

### 文字列・グリッド生成

| メソッド | 説明 |
| --- | --- |
| `rand_string(n, charset = "abcdefghijklmnopqrstuvwxyz")` | 指定文字集合から長さ `n` の文字列を生成 |
| `rand_lower_string(n)` | 長さ `n` の英小文字文字列を生成 |
| `rand_upper_string(n)` | 長さ `n` の英大文字文字列を生成 |
| `rand_binary_string(n)` | 長さ `n` の `'0'`/`'1'` 文字列を生成 |
| `rand_grid(h, w, charset = ".#")` | `h` 行 `w` 列のグリッド（`std::vector<std::string>`）を生成 |

### 出力ヘルパー

- `print_vec(v, os = std::cout, sep = ' ')`
  コンテナの各要素を `sep` 区切りで 1 行出力します。
- `print_grid(grid, os = std::cout)`
  グリッドの各行を順に出力します。

---

## `random_graph.hpp` リファレンス

生成される辺の頂点番号はすべて **0-indexed** です。

### 辺の型

- 重みなし辺: `std::pair<long long, long long>`（`{u, v}`）
- 重み付き辺: `WeightedEdge<T>`（メンバ: `long long u, v; T w;`、デフォルト `T = long long`）

### グラフ生成関数

| 関数 | 生成するグラフ | 辺数の条件 |
| --- | --- | --- |
| `random_tree(rng, n)` | ラベル付き一様ランダム木（Prüfer 列、`O(n)`） | `n <= 1` なら `0` 辺、それ以外は `n - 1` 辺 |
| `random_simple_graph(rng, n, m)` | 単純無向グラフ（自己ループ・多重辺なし） | `0 <= m <= n * (n - 1) / 2` |
| `random_connected_graph(rng, n, m)` | 単純連結無向グラフ | `n == 0` なら `m == 0`、`n >= 1` なら `n - 1 <= m <= n * (n - 1) / 2` |
| `random_directed_graph(rng, n, m)` | 単純有向グラフ（自己ループ・重複辺なし） | `0 <= m <= n * (n - 1)` |
| `random_dag(rng, n, m)` | 単純 DAG（有向非巡回グラフ） | `0 <= m <= n * (n - 1) / 2` |
| `random_multigraph(rng, n, m, directed = false, allow_self_loop = true)` | 多重辺許容グラフ | `m >= 0`（`!allow_self_loop && m > 0` のとき `n >= 2`） |

### 重み付与・隣接リスト変換

- `assign_weights<T = long long>(rng, edges, min_w, max_w)`
  重みなし辺リストに重みを付与して `std::vector<WeightedEdge<T>>` を返します。
  - `T` が整数型の場合: 閉区間 `[min_w, max_w]`
  - `T` が浮動小数点型の場合: 半開区間 `[min_w, max_w)`
  - 例（01-BFS 用）: `assign_weights(rng, edges, 0LL, 1LL)`
  - 例（実数重み）: `assign_weights<double>(rng, edges, 0.0, 10.0)`
- `to_adj_list(n, edges, directed = false)`
  - 重みなし辺リスト -> `std::vector<std::vector<long long>>`
  - 重み付き辺リスト -> `std::vector<std::vector<std::pair<long long, T>>>`

### 出力ヘルパー

出力関数はデフォルトで `index_base = 1`（1-indexed）で出力します。0-indexed で出力したい場合は `index_base` に `0` を指定してください。

- `print_edges(edges, os = std::cout, index_base = 1)`
  各辺を `u v`（重み付きの場合は `u v w`）形式で出力します。
- `print_graph(n, edges, os = std::cout, index_base = 1)`
  1 行目に `N M`、続けて各辺を出力します。
- `print_tree(n, edges, os = std::cout, index_base = 1)`
  1 行目に `N`、続けて各辺を出力します。
