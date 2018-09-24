%{

#include "mydef.h"
#include "y.tab.h"
%}
%option noyywrap
number [0-9]+\.?|[0-9]*\.[0-9]+
%%
[ \t]   { ; }
{number}	{ yylval=atof(yytext);return NUMBER; }
\n|. {return yytext[0];}
