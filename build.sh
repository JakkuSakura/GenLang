#!/bin/sh
echo building lex file
lex genlang.l

echo building yacc file
yacc -d genlang.y

echo compiling
gcc y.tab.c lex.yy.c -ly -lm

echo running
./a.out

