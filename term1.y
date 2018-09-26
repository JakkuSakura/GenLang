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


%union{
    double F64;
    int64_t I64;
    char *STR;
    Node *ND;
    void *NIL;
}
%token<F64> NUMBER
%token<I64> INTEGER
%token<STR> ID
%token<NIL> ADD MUL SUB DIV MOD LSH RSH OR AND XOR

%type<ND> val_expr add_expr mul_expr
%%
lines	:	lines expr '\n'	{ printf("%g\n", $2); }
	|	lines '\n'
    |
	;
expr : val_expr
    ;
add_expr : val_expr
        | val_expr ADD val_expr { $$ = new_node(); $$->type = ADD; $$->un..childs}
val_expr : NUMBER { $$ = new_node(); $$->type = NUMBER; $$->un.val.F64 = $1; }
    | INTEGER { $$ = new_node(); $$->type = INTEGER; $$->un.val.I64 = $1; }
    | ID { $$ = new_node(); $$->type = ID; $$->un.val.STR = strdup($1); }
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
  yyparse();
}

int main(int argc, char **argv)
{
    progname = argv[0];
    yyparse();
}