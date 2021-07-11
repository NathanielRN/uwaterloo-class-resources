cmake_minimum_required(VERSION 3.9)
project(hashtable)

set(CMAKE_CXX_STANDARD 11)

file(GLOB project4 "*.cpp" "*.h")

add_executable(hashtable main.cpp ${project4})