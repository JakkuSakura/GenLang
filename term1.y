%{
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include "mydef.h"


void warning( char *s , char *t );

void yyerror( char *s );
int yylex (void);
%}

%token NUMBER
%%
lines	:	lines expr '\n'	{ printf("%g\n", $2); }
	|	lines '\n'
	|
	;
expr: NUMBER 
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