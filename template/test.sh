while true; do
  ./gen > in.txt
  ./main < in.txt > out1.txt
  ./naive < in.txt > out2.txt
  if ! diff out1.txt out2.txt; then
    echo "found"
    break
  fi
done