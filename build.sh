#!/bin/sh
echo building lex file
lex ./bc.l
echo building yacc file
yacc -d ./term1.y
echo compiling
gcc y.tab.c -lm
./a.out
