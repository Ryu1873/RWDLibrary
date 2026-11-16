// スペシャルジャッジ（複数解問題）用チェッカーテンプレート
// 呼び出し形式: ./checker <in_file> <out_file>
// 終了コード: 0 = AC, 非0 = WA

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using ll = long long;

[[noreturn]] void ac() {
    std::exit(0);
}

[[noreturn]] void wa(const std::string& msg) {
    std::cerr << "WA: " << msg << '\n';
    std::exit(1);
}

void ensure(bool cond, const std::string& msg = "condition failed") {
    if (!cond) wa(msg);
}

template <typename T>
T read_out(std::ifstream& fout) {
    T val{};
    if (!(fout >> val)) {
        wa("unexpected EOF or invalid token in output");
    }
    return val;
}

void ensure_eof(std::ifstream& fout) {
    std::string extra;
    if (fout >> extra) {
        wa("extra token found at end of output: " + extra);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <in_file> <out_file>\n";
        return 2;
    }

    std::ifstream fin(argv[1]);
    std::ifstream fout(argv[2]);
    if (!fin || !fout) {
        std::cerr << "Failed to open input/output file\n";
        return 2;
    }

    // 1. fin から問題の入力を読み込む
    // ll n;
    // fin >> n;

    // 2. fout から main.cpp の出力を読み込む
    // ll x = read_out<ll>(fout);

    // 3. 問題条件を満たすか検証する
    // ensure(1 <= x && x <= n, "output is out of range");

    // 4. 余分な出力がないことを確認して AC
    ensure_eof(fout);
    ac();
}
