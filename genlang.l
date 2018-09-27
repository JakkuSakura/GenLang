%option noyywrap
%{
#include "Node.h"
%}
integer [0-9]+
number [0-9]*\.[0-9]+
identifier [_a-zA-Z][0-9a-zA-Z_]*
%%
[ \t]           { ; }
{identifier}    {
                    yylval = new_node(ID);
                    yylval->STR=strdup(yytext);

                    return ID;
                }
{number}	    {
                    yylval = new_node(NUMBER);

                    yylval->F64=atof(yytext);

                    return NUMBER;
                 }
{integer}       {
                    yylval = new_node(INTEGER);

                    yylval->I64=atol(yytext);

                    return INTEGER;
                 }
"+"             { yylval = new_node_operation(ADD); return ADD; }
"*"             { yylval = new_node_operation(MUL); return MUL; }
"-"             { yylval = new_node_operation(SUB); return SUB; }
"/"             { yylval = new_node_operation(DIV); return DIV; }
"%"             { yylval = new_node_operation(MOD); return MOD; }
"<<"            { yylval = new_node_operation(LSH); return LSH; }
">>"            { yylval = new_node_operation(RSH); return RSH; }
\n|.            { return yytext[0]; }

%%
