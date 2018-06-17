cmake_minimum_required(VERSION 3.9)
project(PageRank)

set(CMAKE_CXX_STANDARD 11)

add_executable(tp3 main.cpp rdtsc.h ppmloader.cpp ppmloader.h)