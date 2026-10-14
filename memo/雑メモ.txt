# テンプレート設定方法
library/template/main.cppを書き換え，以下のコマンドを打つ．
cp ~/kyopro/RWDLibrary/template/main.cpp ~/Library/Preferences/atcoder-cli-nodejs/cpp/main.cpp

# ログインのセッション再接続
1.atcoderにブラウザからログインし，f12→application→cookies→https://atcoder.jp→REVEL_SESSIONの値をコピー
2.(Windowsの場合)~/.config/atcoder-cli-nodejs/session.jsonのREVEL_SESSIONを書き換える．
  (Macの場合)~/Library/Preferences/atcoder-cli-nodejs/session.jsonのREVEL_SESSIONを書き換える．
3.(Windowsの場合)~/.local/share/online-judge-tools/cookie.jarのREVEL_SESSIONを書き換える．
  (Macの場合)~/Library/Application Support/online-judge-tools/cookie.jarのREVEL_SESSIONを書き換える．

# エイリアス/関数設定方法
~/.bashrcを書き換える．

# コマンド
ts <1> カレントディレクトリ直下のディレクトリ<1>にあるmain.cppをコンパイル→テストケースをテスト．
mk ABCxxx 新しいディレクトリを作り，そこに移動．