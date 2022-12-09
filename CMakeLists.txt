cmake_minimum_required(VERSION 3.24)
project(conjg)

set(CMAKE_CXX_STANDARD 17)

add_executable(conjg main.cpp CGSolver.cpp COO2CSR.cpp COO2CSR.hpp CGSolver.hpp)
