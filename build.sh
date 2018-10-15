#!/bin/sh
bison -d -o genlang.y.cpp genlang.y
flex -o genlang.lex.cpp genlang.l

g++ *.cpp -o genlang
