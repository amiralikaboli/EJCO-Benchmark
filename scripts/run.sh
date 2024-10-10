./check_progs.sh || exit

cd ..
export sdql_benchmark_root=$(pwd)

from_root="timings/$1/wcoj/$2/$3_results"
mkdir -p $from_root
echo "*" > "$from_root/.gitignore"

cd "$(realpath progs/)/../generated" || exit
for file in ../progs/$1/$3/*.sdql; do
  name=${file##*/}
  no_ext="${name%.*}"
  echo $no_ext
  echo "Running $no_ext"
  ./$no_ext.out | grep --text " ms" > $sdql_benchmark_root/$from_root/$no_ext.result
done
