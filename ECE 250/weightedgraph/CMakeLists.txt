cmake_minimum_required(VERSION 3.9)
project(weightedgraph)

set(CMAKE_CXX_STANDARD 11)

file(GLOB project5 "*.cpp" "*.h")

add_executable(weightedgraph main.cpp ${project5})