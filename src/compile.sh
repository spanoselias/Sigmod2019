#!/bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

cd $DIR
echo $DIR
mkdir -p build/release
gcc -O3 psort.c -m64 -Wattributes -lpthread -o build/release/sort
