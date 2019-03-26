cmake_minimum_required(VERSION 3.10)
project(SortingAlgorithm)

set(CMAKE_CXX_STANDARD 17)
SET(GCC_COVERAGE_COMPILE_FLAGS "-lpthread -lm")


#add_executable(SortingAlgorithm src/sort.cpp src/psort.c src/utils.h src/utils.c src/psort.c)
add_executable(SortingAlgorithm src/pQuickSort.c)
target_link_libraries(SortingAlgorithm ${GCC_COVERAGE_COMPILE_FLAGS} )

#add_executable(SortingAlgorithm DataStructures/structures.h Utils/Timer.c Sorting/get_time.c Sorting/nomp_mergesort.c)