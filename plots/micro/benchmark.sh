#!/bin/bash

set -e

clang++ micro.cpp -std=c++17 -O3 -march=native -mtune=native -Wno-narrowing -ftree-vectorize -o out

SIZES=(10 100 1000 10000 100000)
REPS=(1)
ITERS=10

filename="results.csv"
echo "R,S,rep,hash,sort" > $filename
for R_size in "${SIZES[@]}"
do
  for S_size in "${SIZES[@]}"
  do
    for rep in "${REPS[@]}"
    do
      echo "$R_size $S_size $rep"
      ./out $R_size $S_size $rep $ITERS >> $filename
    done
  done
done
