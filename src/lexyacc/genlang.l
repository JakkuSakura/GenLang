%{
#include <string>
#include "node.h"
#include "genlang.parser.hh"

#define SAVE_TOKEN()  yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t)    (yylval.token = t)
void count();
%}

%option noyywrap

%%

[ \t\n]					        count();
"//".*"\n"						count();
"extern"                        count(); return TOKEN(TEXTERN);
"import"						count(); return TOKEN(TIMPORT);
"return"				        count(); return TOKEN(TRETURN);
"if"							count(); return TOKEN(TIF);
"for"							count(); return TOKEN(TFOR);
"while" 						count(); return TOKEN(TWHILE);
"continue"						count(); return TOKEN(TCONTINUE);
"break"							count(); return TOKEN(TBREAK);
"int"							count(); SAVE_TOKEN(); return TVARTYPE;
"double" 						count(); SAVE_TOKEN(); return TVARTYPE;
"char" 							count(); SAVE_TOKEN(); return TVARTYPE;
"string"						count(); SAVE_TOKEN(); return TVARTYPE;
"void"	 						count(); SAVE_TOKEN(); return TVARTYPE;
[a-zA-Z_][a-zA-Z0-9_]*  		count(); SAVE_TOKEN(); return TIDENTIFIER;
[0-9]+\.[0-9]* 			    	count(); SAVE_TOKEN(); return TDOUBLE;
[0-9]+					        count(); SAVE_TOKEN(); return TINTEGER;


"="						        count(); return TOKEN(TEQUAL);
"=="				          	count(); return TOKEN(TCEQ);
"!="			          		count(); return TOKEN(TCNE);
"<"				          		count(); return TOKEN(TCLT);
"<="	          				count(); return TOKEN(TCLE);
">"				          		count(); return TOKEN(TCGT);
">="					        count(); return TOKEN(TCGE);

"("	          					count(); return TOKEN(TLPAREN);
")"					          	count(); return TOKEN(TRPAREN);
"{"         					count(); return TOKEN(TLBRACE);
"}"					          	count(); return TOKEN(TRBRACE);

"."         					count(); return TOKEN(TDOT);
","				          		count(); return TOKEN(TCOMMA);

"+"				          		count(); return TOKEN(TPLUS);
"-"		          				count(); return TOKEN(TMINUS);
"*"		          				count(); return TOKEN(TMUL);
"/"				          		count(); return TOKEN(TDIV);
";"								count(); return TOKEN(TENDSTATEMENT);
"\""("\\\""|[^"\""])*"\""		count(); SAVE_TOKEN(); return TSTRING;
"\'".+"\'"						count(); SAVE_TOKEN(); return TCHAR;
.                     			count(); printf("Unknown token!\n"); yyterminate();

%%
int column = 0;

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	/* ECHO; */
}
