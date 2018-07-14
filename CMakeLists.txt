cmake_minimum_required(VERSION 3.10)
project(pretty_vector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

add_executable(pretty_vector tests.h tests.cpp vector.h)