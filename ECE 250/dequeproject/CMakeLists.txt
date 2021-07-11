cmake_minimum_required(VERSION 3.9)
project(deqeueproject)

set(CMAKE_CXX_STANDARD 11)
file(GLOB project2 "*.h" "*.cpp")

add_executable(dequeproject ${project2} devmain.cpp AutomatedTest.cpp)