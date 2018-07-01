cmake_minimum_required(VERSION 3.10)
project(Código)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(Test
        calcular_rayos.cpp
        calcular_rayos.h
        ppmloader.cpp
        ppmloader.h
        rdtsc.h
        test_main.cpp
        util.cpp
        util.h
        VectorMapMatrix.cpp
        VectorMapMatrix.h)

add_executable(TP3
        calcular_rayos.cpp
        calcular_rayos.h
        ppmloader.cpp
        ppmloader.h
        rdtsc.h
        main.cpp
        util.cpp
        util.h
        VectorMapMatrix.cpp
        VectorMapMatrix.h)

add_executable(matlab
        main_dibujar_rectas_matlab.cpp
        calcular_rayos.cpp
        ppmloader.cpp
        rdtsc.h
        util.cpp
        VectorMapMatrix.cpp)