%{

#include "mydef.h"
#include "y.tab.h"
%}
%option noyywrap
integer [0-9]+
number [0-9]*\.[0-9]+
identifier [_a-zA-Z][0-9a-zA-Z]*
%%
[ \t]           { ; }
{identifier}    { yylval.STR=strdup(yytext);return ID; }
{number}	    { yylval.F64=atof(yytext);return NUMBER; }
{integer}       { yylval.I64=atol(yytext);return INTEGER; }
\n|.            { return yytext[0]; }
