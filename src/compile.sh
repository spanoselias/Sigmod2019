#!/bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

cd $DIR
echo $DIR
mkdir -p build/release
#g++ -std=c++11 -O3 psort.c -m64 -lpthread -fpermissive -o build/release/sort
#gcc -O3 psort.c -m64 -lpthread -o build/release/sort
gcc -O3 pbitonic.c -m64  -fopenmp  -o build/release/sort -lm