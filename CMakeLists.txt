cmake_minimum_required(VERSION 3.10)
project(SortingAlgorithm)

set(CMAKE_CXX_STANDARD 17)

add_executable(SortingAlgorithm DataStructures/structures.h main.cpp Utils/Timer.c  Utils/files.c)
#add_executable(SortingAlgorithm DataStructures/structures.h main.cpp Utils/Timer.c Sorting/get_time.c Sorting/nomp_mergesort.cpp)
#add_executable(SortingAlgorithm DataStructures/structures.h Utils/Timer.c Sorting/get_time.c Sorting/nomp_mergesort.c)