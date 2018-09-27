%{
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include "Node.h"

void warning( char *s , char *t );

void yyerror( char *s );
int yylex (void);
%}

%token NUMBER INTEGER ID
%token ADD MUL SUB DIV MOD LSH RSH OR AND XOR NODE
%%
lines :     line lines
        |   line
        ;
line : add_expr '\n' {printf("a line\n");}
    | '\n'
    ;
add_expr : mul_expr
        | mul_expr ADD mul_expr {
            $$ = new_node();
            $$->type = NODE;
            $$->un.expr.token = ADD;
            $$->un.expr.len = 2;
            $$->un.expr.childs = new_node_array(2);
            $$->un.expr.childs[0] = $1;
            $$->un.expr.childs[1] = $3;

        }
        ;

mul_expr : val_expr
        | val_expr MUL val_expr {
            $$ = new_node();
            $$->type = NODE;
            $$->un.expr.token = MUL;
            $$->un.expr.len = 2;
            $$->un.expr.childs = new_node_array(2);
            $$->un.expr.childs[0] = $1;
            $$->un.expr.childs[1] = $3;

        }
        ;

val_expr : NUMBER   {printf("read number %g\n", $1->un.val.F64);}
    | INTEGER       {printf("read integer %lld\n", $1->un.val.I64);}
    | ID            {printf("read id %s\n", $1->un.val.STR);}
    ;
%%

#include <stdio.h>
#include <ctype.h>

#include <string.h>
char *progname;
void warning( char *s , char *t )
{
  fprintf( stderr ,"%s: %s\n" , progname , s );
  if ( t )
    fprintf( stderr , " %s\n" , t );
}
void yyerror( char *s )
{
  warning( s , ( char * )0 );
}

int main(int argc, char **argv)
{
    progname = argv[0];
    yyparse();
}