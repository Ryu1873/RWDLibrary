#!/usr/bin/env bash
set -euo pipefail

# デフォルト設定
MAX_CASES=0          # 0 = 無限 (Ctrl+C で停止)
TIMEOUT_SEC="2.0"
USE_SANITIZER=0

usage() {
  cat <<'EOF'
Usage: test_judge.sh [options]
  Stress-test ./main (main.cpp) using ./gen (gen.cpp) and ./checker (checker.cpp).
  Designed for problems with multiple valid answers (special judge).
  Checker interface: ./checker in.txt out1.txt (exit 0 = AC, non-zero = WA)

Options:
  -n <count>   Maximum number of test cases (default: 0 = unlimited)
  -t <sec>     Time limit in seconds per case (default: 2.0)
  -s           Enable Address/UndefinedBehavior Sanitizers for main.cpp
  -h           Show this help message
EOF
}

while getopts "n:t:sh" opt; do
  case "$opt" in
    n) MAX_CASES="$OPTARG" ;;
    t) TIMEOUT_SEC="$OPTARG" ;;
    s) USE_SANITIZER=1 ;;
    h) usage; exit 0 ;;
    *) usage; exit 2 ;;
  esac
done

# カラー出力設定 (TTY のみ)
if [ -t 1 ]; then
  C_RESET=$'\033[0m'
  C_RED=$'\033[31m'
  C_GREEN=$'\033[32m'
  C_YELLOW=$'\033[33m'
  C_CYAN=$'\033[36m'
  C_BOLD=$'\033[1m'
else
  C_RESET="" C_RED="" C_GREEN="" C_YELLOW="" C_CYAN="" C_BOLD=""
fi

# RWDLibrary ルートの解決 (コピー先からの実行にも対応)
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
if [ -n "${RWD_LIBRARY_DIR:-}" ] && [ -d "$RWD_LIBRARY_DIR/random" ]; then
  LIB_ROOT="$RWD_LIBRARY_DIR"
elif [ -d "$SCRIPT_DIR/../random" ]; then
  LIB_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
elif [ -d "$HOME/kyopro/RWDLibrary/random" ]; then
  LIB_ROOT="$HOME/kyopro/RWDLibrary"
else
  LIB_ROOT="$SCRIPT_DIR"
fi

CXX="${CXX:-$(command -v g++-16 || command -v g++ || true)}"
if [ -z "$CXX" ]; then
  echo "${C_RED}Error: C++ compiler (g++-16 / g++) not found.${C_RESET}" >&2
  exit 1
fi

for f in main.cpp gen.cpp checker.cpp; do
  if [ ! -f "$f" ]; then
    echo "${C_RED}Error: $f not found in $(pwd).${C_RESET}" >&2
    exit 1
  fi
done

COMMON_FLAGS=(-std=c++23 -O2 -Wall -Wextra -I"$LIB_ROOT")
MAIN_FLAGS=("${COMMON_FLAGS[@]}")
if [ "$USE_SANITIZER" -eq 1 ]; then
  MAIN_FLAGS+=(-g -fsanitize=address,undefined -fno-omit-frame-pointer)
fi

echo "${C_CYAN}[Compile]${C_RESET} Using $CXX (include: $LIB_ROOT)"
"$CXX" "${MAIN_FLAGS[@]}" main.cpp -o main
"$CXX" "${COMMON_FLAGS[@]}" gen.cpp -o gen
"$CXX" "${COMMON_FLAGS[@]}" checker.cpp -o checker
echo "${C_GREEN}[Compile]${C_RESET} Done."

run_with_limit() {
  local tlim="$1"
  local infile="$2"
  local outfile="$3"
  local errfile="$4"
  local cmd="$5"
  perl -MTime::HiRes=gettimeofday,tv_interval,setitimer,ITIMER_REAL -e '
    use POSIX qw(WIFEXITED WEXITSTATUS WIFSIGNALED WTERMSIG EINTR);
    my ($tlim, $in, $out, $err, $cmd) = @ARGV;
    my $t0 = [gettimeofday];
    my $pid = fork();
    die "fork failed: $!" unless defined $pid;
    if ($pid == 0) {
      open(STDIN, "<", $in) or exit 127;
      open(STDOUT, ">", $out) or exit 127;
      open(STDERR, ">", $err) or exit 127;
      exec($cmd) or exit 127;
    }
    my $timed_out = 0;
    $SIG{ALRM} = sub {
      $timed_out = 1;
      kill("KILL", $pid);
    };
    setitimer(ITIMER_REAL, $tlim) if $tlim > 0;
    while (waitpid($pid, 0) <= 0) {
      last if $! != EINTR;
    }
    my $status = $?;
    setitimer(ITIMER_REAL, 0) if $tlim > 0;
    my $ms = int(tv_interval($t0) * 1000 + 0.5);
    if ($timed_out) {
      print "TLE $ms\n";
      exit 124;
    }
    if (WIFSIGNALED($status)) {
      my $sig = WTERMSIG($status);
      print "SIG($sig) $ms\n";
      exit(128 + $sig);
    }
    my $code = WEXITSTATUS($status);
    print "OK $ms\n";
    exit($code);
  ' "$tlim" "$infile" "$outfile" "$errfile" "$cmd"
}

show_preview() {
  local title="$1"
  local file="$2"
  local max_lines="${3:-40}"
  if [ ! -s "$file" ]; then
    echo "${C_BOLD}--- $title (empty) ---${C_RESET}"
    return
  fi
  local total
  total=$(wc -l < "$file" | tr -d ' ')
  echo "${C_BOLD}--- $title ($total lines) ---${C_RESET}"
  head -n "$max_lines" "$file"
  if [ "$total" -gt "$max_lines" ]; then
    echo "... ($((total - max_lines)) more lines in $file)"
  fi
}

CASE_NUM=0
PASSED=0
MAX_MS=0
TOTAL_MS=0

cleanup_on_interrupt() {
  if [ -t 1 ]; then echo ""; fi
  local avg_ms=0
  if [ "$PASSED" -gt 0 ]; then
    avg_ms=$((TOTAL_MS / PASSED))
  fi
  echo "${C_YELLOW}[Stopped]${C_RESET} Passed $PASSED cases (max: ${MAX_MS} ms, avg: ${avg_ms} ms)"
  exit 130
}
trap cleanup_on_interrupt INT TERM

while [ "$MAX_CASES" -eq 0 ] || [ "$CASE_NUM" -lt "$MAX_CASES" ]; do
  CASE_NUM=$((CASE_NUM + 1))

  if ! ./gen > in.txt 2> gen_err.txt; then
    if [ -t 1 ]; then echo ""; fi
    echo "${C_RED}${C_BOLD}[GEN_ERR]${C_RESET} Case #$CASE_NUM: ./gen failed"
    show_preview "gen stderr" gen_err.txt
    exit 1
  fi

  rc=0
  if info="$(run_with_limit "$TIMEOUT_SEC" in.txt out1.txt main_err.txt ./main)"; then
    rc=0
  else
    rc=$?
  fi
  status="${info%% *}"
  ms="${info##* }"
  if [ "$ms" -gt "$MAX_MS" ]; then MAX_MS="$ms"; fi
  TOTAL_MS=$((TOTAL_MS + ms))

  if [ "$rc" -ne 0 ]; then
    if [ -t 1 ]; then echo ""; fi
    if [ "$status" = "TLE" ]; then
      echo "${C_RED}${C_BOLD}[TLE]${C_RESET} Case #$CASE_NUM (${ms} ms > ${TIMEOUT_SEC} s)"
    else
      echo "${C_RED}${C_BOLD}[RE]${C_RESET} Case #$CASE_NUM (status: $status, exit: $rc, ${ms} ms)"
    fi
    show_preview "stderr (main)" main_err.txt
    show_preview "input (in.txt)" in.txt
    echo "${C_CYAN}Reproduce:${C_RESET} ./main < in.txt"
    exit 1
  fi

  if ! ./checker in.txt out1.txt > checker_out.txt 2> checker_err.txt; then
    if [ -t 1 ]; then echo ""; fi
    echo "${C_RED}${C_BOLD}[WA]${C_RESET} Case #$CASE_NUM (${ms} ms)"
    show_preview "checker message" checker_err.txt
    show_preview "input (in.txt)" in.txt
    show_preview "output (out1.txt)" out1.txt
    echo "${C_CYAN}Reproduce:${C_RESET} ./main < in.txt > out1.txt && ./checker in.txt out1.txt"
    exit 1
  fi

  PASSED=$((PASSED + 1))
  avg_ms=$((TOTAL_MS / PASSED))
  if [ -t 1 ]; then
    printf "\r${C_GREEN}[PASS]${C_RESET} %d cases | last: %d ms | max: %d ms | avg: %d ms   " \
      "$PASSED" "$ms" "$MAX_MS" "$avg_ms"
  elif [ $((PASSED % 100)) -eq 0 ] || [ "$PASSED" -eq "$MAX_CASES" ]; then
    printf "[PASS] %d cases | last: %d ms | max: %d ms | avg: %d ms\n" \
      "$PASSED" "$ms" "$MAX_MS" "$avg_ms"
  fi
done

if [ -t 1 ]; then echo ""; fi
avg_ms=0
if [ "$PASSED" -gt 0 ]; then
  avg_ms=$((TOTAL_MS / PASSED))
fi
echo "${C_GREEN}${C_BOLD}[AC]${C_RESET} All $PASSED cases passed! (max: ${MAX_MS} ms, avg: ${avg_ms} ms)"
