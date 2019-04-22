cmake_minimum_required(VERSION 3.13)
project(sigmod2 C)

set(CMAKE_C_STANDARD 11)

add_executable(sigmod2 main.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(sigmod2 Threads::Threads)