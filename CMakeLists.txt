cmake_minimum_required(VERSION 3.0)
INCLUDE_DIRECTORIES(includes)
add_executable(genlang src/GC.cpp src/parser.cpp src/scanner.cpp)
