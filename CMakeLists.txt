cmake_minimum_required(VERSION 3.14)
project(lab_mc458)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(lab_mc458
    instance.h
    main.cpp
    main.h
    point.h
    solver.cpp
    solver.h)
