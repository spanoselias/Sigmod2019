#!/bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

cd $DIR
echo $DIR
mkdir -p build/release
gcc  sort.cpp pQuickSort.c -lpthread -o build/release/sort
