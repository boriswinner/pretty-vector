cmake_minimum_required(VERSION 3.10)
project(pretty_vector)

set(CMAKE_CXX_STANDARD 11)

add_executable(pretty_vector tests.h tests.cpp vector.h)