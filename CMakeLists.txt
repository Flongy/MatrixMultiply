cmake_minimum_required(VERSION 3.15)
project(ParallelProgram2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fopenmp")

add_executable(ParallelProgram2 main.cpp Timer.h Matrix.h)