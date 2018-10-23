%{
#include "node.h"
#include <cstdio>
#include <cstdlib>
NProgram *programBlock = NULL; /* the top level root node of our final AST */

extern int yylex();
extern char *yytext;
void yyerror(const char *s);
%}

/* Represents the many different ways we can access our data */
%union {
	Node *node;
	NBlock *block;
	NExpression *expr;
	NStatement *stmt;
	NIdentifier *ident;
	NString *nstr;
	NVarType *vartype;
	NProgram *program;
	NVariableDeclaration *var_decl;
	std::vector<NVariableDeclaration*> *varvec;
	std::vector<NExpression*> *exprvec;
	std::string *string;
	int token;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */
%token <string> TIDENTIFIER TINTEGER TDOUBLE TVARTYPE TSTRING
%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL TENDSTATEMENT
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT
%token <token> TPLUS TMINUS TMUL TDIV
%token <token> TRETURN TEXTERN
%token <token> TIF TFOR TWHILE TCONTINUE TBREAK

/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type <ident> ident
%type <expr> numeric expr
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> stmts block
%type <program> program
%type <stmt> stmt var_decl_stmt func_decl extern_decl if_stmt loop_stmt
%type <var_decl> var_decl
%type <token> comparison assign
%type <vartype> var_type
%type <nstr> const_string

/* Operator precedence for mathematical operators */
%left TPLUS TMINUS
%left TMUL TDIV

%start program

%%

program : stmts { programBlock = new NProgram(); programBlock->setBlock($1); }
		;

stmts : stmt { $$ = new NBlock(); $$->statements.push_back($1); }
	  | stmts stmt { $1->statements.push_back($2); }
	  ;

stmt : func_decl
	 | extern_decl
	 | expr TENDSTATEMENT { $$ = new NExpressionStatement(*$1); }
	 | TRETURN expr TENDSTATEMENT { $$ = new NReturnStatement(*$2); }
	 | if_stmt
	 | loop_stmt
	 | var_decl_stmt
	 | TENDSTATEMENT { $$ = new NStatement(); }
     ;
if_stmt : TIF TLPAREN expr TRPAREN stmt { $$ = new NIfStatement(*$3, *$5); }
	| TIF TLPAREN expr TRPAREN block { $$ = new NIfStatement(*$3, *$5); }
	;
loop_stmt
	:	 TFOR TLPAREN var_decl TENDSTATEMENT expr TENDSTATEMENT expr TRPAREN stmt
			{  $$ = new NLoopStatement(*$3, *$5, *$7, *$9); }
	|	TFOR TLPAREN var_decl TENDSTATEMENT expr TENDSTATEMENT expr TRPAREN block
			{  $$ = new NLoopStatement(*$3, *$5, *$7, *$9); }
	|	TFOR TLPAREN expr TENDSTATEMENT expr TENDSTATEMENT expr TRPAREN stmt
			{ $$ = new NLoopStatement(*$3, *$5, *$7, *$9); }
	|	TFOR TLPAREN expr TENDSTATEMENT expr TENDSTATEMENT expr TRPAREN block
			{  $$ = new NLoopStatement(*$3, *$5, *$7, *$9); }
	;

block : TLBRACE stmts TRBRACE { $$ = $2; }
	  | TLBRACE TRBRACE { $$ = new NBlock(); }
	  ;
var_decl_stmt : var_decl TENDSTATEMENT { $$ = new NVariableDeclarationStatement(*$1); }
	;

var_decl : var_type ident { $$ = new NVariableDeclaration(*$1, *$2); }
	| var_type ident TEQUAL expr { $$ = new NVariableDeclaration(*$1, *$2, $4); }
	;

extern_decl : TEXTERN var_type ident TLPAREN func_decl_args TRPAREN TENDSTATEMENT
                { $$ = new NExternDeclaration(*$2, *$3, *$5); delete $5; }
            ;

func_decl : var_type ident TLPAREN func_decl_args TRPAREN block
			{ $$ = new NFunctionDeclaration(*$1, *$2, *$4, *$6); delete $4; }
		  ;

func_decl_args : /*blank*/  { $$ = new VariableList(); }
		  | var_decl { $$ = new VariableList(); $$->push_back($1); }
		  | func_decl_args TCOMMA var_decl { $1->push_back($3); }
		  ;
var_type : TVARTYPE		{ $$ = new NVarType(*$1); delete $1; }
			;
ident : TIDENTIFIER 	{ $$ = new NIdentifier(*$1); delete $1; }
	  ;

numeric : TINTEGER { $$ = new NInteger(atol($1->c_str())); delete $1; }
		| TDOUBLE { $$ = new NDouble(atof($1->c_str())); delete $1; }
		;

expr : ident assign expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
	 | ident TLPAREN call_args TRPAREN { $$ = new NMethodCall(*$1, *$3); delete $3; }
	 | ident { $$ = $1; }
	 | numeric
         | expr TMUL expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
         | expr TDIV expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
         | expr TPLUS expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
         | expr TMINUS expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
 	 | expr comparison expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | TLPAREN expr TRPAREN { $$ = $2; }
	 | const_string	{ $$ = $1; }
	 ;
assign : TEQUAL
	;
const_string : TSTRING { $$ = new NString(*$1); delete $1; }
	;
call_args : /*blank*/  { $$ = new ExpressionList(); }
		  | expr { $$ = new ExpressionList(); $$->push_back($1); }
		  | call_args TCOMMA expr  { $1->push_back($3); }
		  ;

comparison : TCEQ | TCNE | TCLT | TCLE | TCGT | TCGE;

%%

void yyerror(const char *s) { std::fprintf(stdout, "Error: %s \"%s\"\n", s, yytext);  }
