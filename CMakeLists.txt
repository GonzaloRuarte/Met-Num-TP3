cmake_minimum_required(VERSION 3.9)
project(PageRank)

set(CMAKE_CXX_STANDARD 11)

add_executable(tp3 main.cpp util.cpp rdtsc.h ppmloader.cpp ppmloader.h calcular_rayos.cpp calcular_rayos.h VectorMapMatrix.cpp)

add_executable(testcalcular_rayos test_main.cpp )