./check_progs.sh || exit

cd "$(realpath ../progs/)/.." || exit

for file in progs/$1/*.sdql; do
  name=${file##*/}
  no_ext="${name%.*}"
  rm -f generated/$no_ext.cpp
done

qs=( $(ls progs/$1/ | grep '.sdql' ) )
sbt "run benchmark $2 progs/$1 $(printf '%q ' "${qs[@]}")"
