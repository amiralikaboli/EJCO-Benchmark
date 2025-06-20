./check_progs.sh || exit

cd "$(realpath ../progs/)/../generated" || exit

for file in ../progs/$1/*.sdql; do
  name=${file##*/}
  no_ext="${name%.*}"
  rm -f no_ext.out
done

for file in ../progs/$1/*.sdql; do
  name=${file##*/}
  no_ext="${name%.*}"
  echo "Compiling $no_ext"
  clang++ -std=c++17 -O3 -march=native -mtune=native -Wno-narrowing -ftree-vectorize -Wno-deprecated-builtins $no_ext.cpp -o $no_ext.out
done
