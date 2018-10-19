#include <iostream>
#include "work.h"
#include "node.h"

using namespace std;

extern int yyparse();
extern NBlock* programBlock;
extern char *yytext;
int main(int argc, char **argv)
{
	printf("%s\n", "Start your show now!");

	yyparse();
	printf("%s\n", "Parsed");
	if(programBlock)
	{
		cout << programBlock << endl;
		print(programBlock, 0);
	}
	return 0;
}
