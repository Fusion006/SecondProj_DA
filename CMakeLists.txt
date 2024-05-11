cmake_minimum_required(VERSION 3.26)
project(Second_Proj_DA)

set(CMAKE_CXX_STANDARD 17)

add_executable(Second_Proj_DA main.cpp
        Graph.h
        Graph.cpp
        haversine.h
        haversine.cpp
        antColOpt.cpp
        Backtracking.cpp)
