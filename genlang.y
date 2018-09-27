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
Node *alloc_childs_two(Node *mid, Node *lhs, Node *rhs)
{
    mid->len = 2;
    mid->childs = new_pNode_array(2);
    mid->childs[0] = lhs;
    mid->childs[1] = rhs;
}
pNode root;
void init()
{
    root = new_node(NODE);
    root->token = BLOCK;
}
void eval(pNode r)
{
    if(r->type == NODE)
    {
        switch (r->token)
        {
            case BLOCK:
                for (int i = 0; i < r->len; ++i) {
                    eval(r->childs[i]);
                }
                break;
        }
    }
}
%}

%token NUMBER INTEGER ID
%token ADD MUL SUB DIV MOD LSH RSH OR AND XOR
%token NODE BLOCK
%%
lines :
        |   lines line { add_child_node(root, $2);}
        ;
line : shift_expr '\n' {}
    | '\n'
    ;

shift_expr : add_expr
        | mul_expr LSH mul_expr { $$ = alloc_childs_two($2, $1, $3); }
        | mul_expr RSH mul_expr { $$ = alloc_childs_two($2, $1, $3); }
        ;
add_expr : mul_expr
        | mul_expr ADD mul_expr { $$ = alloc_childs_two($2, $1, $3); }
        | mul_expr SUB mul_expr { $$ = alloc_childs_two($2, $1, $3); }
        ;

mul_expr : val_expr
        | val_expr MUL val_expr { $$ = alloc_childs_two($2, $1, $3); }
        | val_expr DIV val_expr { $$ = alloc_childs_two($2, $1, $3); }
        | val_expr MOD val_expr { $$ = alloc_childs_two($2, $1, $3); }
        ;

val_expr : NUMBER   /*{printf("read number %g\n", $1->un.val.F64);}*/
    | INTEGER       /*{printf("read integer %lld\n", $1->un.val.I64);}*/
    | ID            /*{printf("read id %s\n", $1->un.val.STR);}*/
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
    printf("Welcome to use GenLang\nIt's your show now!\n");
    init();
    progname = argv[0];
    yyparse();
    free_nodes();
}
