cmake_minimum_required(VERSION 3.9)
project(avltree)

set(CMAKE_CXX_STANDARD 11)

file(GLOB project3 "*.cpp" "*.h")

add_executable(avltree ${project3} main.cpp Exception.h Search_tree.h ece250.h test.cpp bailyTest.cpp)