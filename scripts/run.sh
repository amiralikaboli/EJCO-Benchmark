./check_progs.sh || exit

cd ..
export sdql_benchmark_root=$(pwd)

from_root="timings/$2/wcoj/$3/$4_results"
mkdir -p $from_root
echo "*" > "$from_root/.gitignore"

cd "$(realpath progs/)/../generated" || exit
for file in ../progs/$2/$4/*.sdql; do
  name=${file##*/}
  no_ext="${name%.*}"
  echo $no_ext
  echo "Running $no_ext"
  result_file=$sdql_benchmark_root/$from_root/$no_ext.result
  timeout $1 ./$no_ext.out | grep --text " ms" > $result_file \
  || echo "Timed out after $1 seconds" > $result_file
done
