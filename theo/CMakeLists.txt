cmake_minimum_required(VERSION 3.13)
project(theo C)

set(CMAKE_C_STANDARD 11)

add_executable(theo main.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(theo Threads::Threads)
