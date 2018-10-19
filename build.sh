#!/bin/sh
bison -d -o src/genlang.parser.cc yacc/genlang.y
flex -o src/genlang.scanner.cc lex/genlang.l
mv src/genlang.parser.hh includes/genlang.parser.hh
rm genlang testlex
g++ src/*.cc src/main/*.cpp -o genlang -Iincludes -g
# g++ src/*.cpp test/lex.cpp -o testlex -Iincludes
