# テンプレート設定方法
library/template/main.cppを書き換え，AtCoderディレクトリで以下のコマンドを打つ．
cp library/template/main.cpp ~/.config/atcoder-cli-nodejs/cpp/

# ログインのセッション再接続
1.atcoderにブラウザからログインし，f12→application→cookies→https://atcoder.jp→REVEL_SESSIONの値をコピー
2.~/.config/atcoder-cli-nodejs/session.jsonのREVEL_SESSIONを書き換える．
3.~/.local/share/online-judge-tools/cookie.jarのREVEL_SESSIONを書き換える．

# エイリアス/関数設定方法
~/.bashrcを書き換える．

# コマンド
ts <1> カレントディレクトリ直下のディレクトリ<1>にあるmain.cppをコンパイル→テストケースをテスト．
mk <コンテスト名> 新しいディレクトリを作り，そこに移動．
