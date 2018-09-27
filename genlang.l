%option noyywrap
%{
#include "Node.h"
%}
integer [0-9]+
number [0-9]*\.[0-9]+
identifier [_a-zA-Z][0-9a-zA-Z]*
%%
[ \t]           { ; }
{identifier}    {
                    yylval = new_node();
                    yylval->type = ID;

                    yylval->un.val.STR=strdup(yytext);

                    return ID;
                }
{number}	    {
                    yylval = new_node();
                    yylval->type = NUMBER;

                    yylval->un.val.F64=atof(yytext);

                    return NUMBER;
                 }
{integer}       {
                    yylval = new_node();
                    yylval->type = INTEGER;

                    yylval->un.val.I64=atol(yytext);

                    return INTEGER;
                 }
"+"             { yylval = new_node_operation(ADD); return ADD; }
\n|.            { return yytext[0]; }
%%
