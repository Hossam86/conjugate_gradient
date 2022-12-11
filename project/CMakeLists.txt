cmake_minimum_required(VERSION 3.24)
project(conjugate)

set(CMAKE_CXX_STANDARD 17)

add_executable(conjugate main.cpp CGSolver.cpp CGSolver.hpp matvecops.hpp matvecops.cpp)
