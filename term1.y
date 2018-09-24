%{
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>

void warning( char *s , char *t );

void yyerror( char *s );
int yylex (void);
%}


%union{
    double F64;
    int64_t I64;
    char *STR;
    struct{
        int son_int;
        union YYSTYPE *son;
    }TREE;
}

%token<F64> NUMBER
%token<I64> INTEGER
%token<STR> ID
%type<TREE> TreeNode
%type<F64> expr
%%
lines	:	lines expr '\n'	{ printf("%g\n", $2); }
	|	lines '\n'
    |
	;
expr : NUMBER
    | INTEGER { $$ = (double)$1; }

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