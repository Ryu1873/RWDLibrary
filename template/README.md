# template 使い方ガイド

競技プログラミング用の常用テンプレートおよびランダムテスト（ストレステスト）実行スクリプト集です。

## ファイル構成

| ファイル | 役割 |
| --- | --- |
| `main.cpp` | 解答用 C++23 テンプレート |
| `test.sh` | 通常問題用ランダムテストスクリプト（`main.cpp` と `naive.cpp` の出力を `diff` 比較） |
| `test_judge.sh` | 複数解問題（スペシャルジャッジ）用ランダムテストスクリプト（`checker.cpp` で正当性検証） |
| `checker.cpp` | `test_judge.sh` で使用するスペシャルジャッジ用 C++ テンプレート |

---

## 1. `test.sh`（通常比較用）

愚直解（`naive.cpp`）と本番解（`main.cpp`）の出力を比較してバグを検出します。

### 必要なファイル

作業ディレクトリに以下の 3 ファイルを配置します（`test.sh` も作業ディレクトリにコピーするか、パスを通して実行します）。

- `main.cpp`（検証したい解答コード）
- `naive.cpp`（愚直解など、確実に正しいコード）
- `gen.cpp`（テストケースを標準出力に出力するジェネレータ。[random/](../random/README.md) が利用可能）

### 実行例

```sh
# 無限ループでテスト（Ctrl+C で停止して統計表示）
./test.sh

# 100 ケースだけ実行
./test.sh -n 100

# タイムアウト 1.0 秒、Sanitizer (ASan/UBSan) 有効で 50 ケース実行
./test.sh -n 50 -t 1.0 -s
```

### オプション

| オプション | 説明 | デフォルト |
| --- | --- | --- |
| `-n <count>` | 最大テスト回数（`0` で無限ループ） | `0` |
| `-t <sec>` | 1 ケースあたりのタイムアウト秒数（`naive` には 5 倍を適用） | `2.0` |
| `-s` | `main.cpp` のコンパイルに `-fsanitize=address,undefined` を付与 | 無効 |
| `-w` | 出力比較時に空白差を無視（`diff -uw`） | 無効（厳密 `diff -u`） |
| `-h` | ヘルプを表示 | - |

### 判定と出力ファイル

- **`[AC]` / `[PASS]`**: 通過ケース数・直近/最大/平均実行時間（ms）を表示します。
- **`[WA]`**: `diff -u out2.txt out1.txt` の差分（`diff.txt`）と入力 `in.txt` を表示して停止します。
- **`[RE]` / `[TLE]`**: エラーコード / シグナル・経過時間・`main` の stderr（`main_err.txt`）・`in.txt` を表示して停止します。
- **`[GEN_ERR]` / `[NAIVE_ERR]`**: `gen` または `naive` 実行時のエラーを表示して停止します。

失敗時は作業ディレクトリに `in.txt`, `out1.txt`, `out2.txt` が残るため、`./main < in.txt` でそのままデバッグできます。

---

## 2. `test_judge.sh` + `checker.cpp`（複数解・スペシャルジャッジ用）

「条件を満たす構築例を 1 つ出力せよ」など、答えが複数存在する問題向けのランダムテストです。`naive.cpp` は不要です。

### 必要なファイル

作業ディレクトリに以下の 3 ファイルを配置します。

- `main.cpp`（検証したい解答コード）
- `gen.cpp`（テストケースジェネレータ）
- `checker.cpp`（`template/checker.cpp` をベースに検証ロジックを記述したもの）

### 実行例

```sh
# 無限ループでテスト（Ctrl+C で停止）
./test_judge.sh

# Sanitizer 有効で 200 ケース実行
./test_judge.sh -n 200 -s
```

### オプション

| オプション | 説明 | デフォルト |
| --- | --- | --- |
| `-n <count>` | 最大テスト回数（`0` で無限ループ） | `0` |
| `-t <sec>` | 1 ケースあたりのタイムアウト秒数 | `2.0` |
| `-s` | `main.cpp` のコンパイルに `-fsanitize=address,undefined` を付与 | 無効 |
| `-h` | ヘルプを表示 | - |

### `checker.cpp` の仕様とヘルパー

`test_judge.sh` はチェッカーを `./checker in.txt out1.txt` として呼び出します。  
終了コード `0` を `AC`、非 `0` を `WA` として扱います（`stderr` に出力した理由が失敗時に表示されます）。

`template/checker.cpp` には以下のヘルパーが用意されています。

| ヘルパー | 説明 |
| --- | --- |
| `ac()` | 終了コード `0` で終了（`AC`） |
| `wa(msg)` | `stderr` に `WA: <msg>` を出力して終了コード `1` で終了 |
| `ensure(cond, msg)` | `cond` が `false` なら `wa(msg)` を呼ぶ |
| `read_out<T>(fout)` | `fout` から型 `T` を読み込む（EOF や型不一致なら自動で `wa`） |
| `ensure_eof(fout)` | 末尾に余分な出力トークンが残っていれば自動で `wa` |

#### `checker.cpp` の記述例

「整数 $N$ に対し $a \ge 1, b \ge 1, a + b = N$ を満たす $a, b$ を出力する問題」の例:

```cpp
ll n;
fin >> n;

ll a = read_out<ll>(fout);
ll b = read_out<ll>(fout);

ensure(a >= 1 && b >= 1, "a and b must be positive");
ensure(a + b == n, "a + b != n");

ensure_eof(fout);
ac();
```

---

## 3. 動作環境・カスタマイズ

- **ライブラリパスの自動解決**:
  コンパイル時に `-I"$LIB_ROOT"` が自動で付与されるため、スクリプトを問題ディレクトリにコピーして実行した場合でも `gen.cpp` や `main.cpp` で `#include "random/random.hpp"` 等がそのまま使えます。
  解決の優先順位は以下の通りです。
  1. 環境変数 `RWD_LIBRARY_DIR`（`$RWD_LIBRARY_DIR/random` が存在する場合）
  2. スクリプトの親ディレクトリ（`RWDLibrary/template/` から直接実行した場合など）
  3. `$HOME/kyopro/RWDLibrary`
- **コンパイラ指定**:
  デフォルトでは `g++-16`（見つからない場合は `g++`）を使用します。環境変数 `CXX` で変更できます。

```sh
CXX=g++-15 ./test.sh -n 50
```
