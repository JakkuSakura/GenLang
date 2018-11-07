#include <iostream>
#include <cstdio>
#include "work.h"
#include "node.h"

using namespace std;

extern int yyparse();
extern NProgram* programBlock;
extern char *yytext;
extern FILE *yyin, *yyout;
NProgram *parseProgram(FILE *input_file, FILE *output_file)
{
	yyin = input_file;
	yyout = output_file;
	yyparse();
	return programBlock;
}
int main(int argc, char **argv)
{

	fprintf(stderr, "%s\n", "Start your show now!");
	NProgram *pro = parseProgram(stdin, stdout);
	fprintf(stderr, "%s\n", "Parsed");
	if(pro)
	{
		//print(pro, 0);
		pas_code(pro);
	}
	return 0;
}
