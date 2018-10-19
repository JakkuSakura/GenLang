#include <iostream>
#include "work.h"
#include "node.h"

using namespace std;

extern int yyparse();
extern NBlock* programBlock;

int main(int argc, char **argv)
{
	printf("%s\n", "Start your show now!");
	yyparse();
	printf("%s\n", "Parsed");
	cout << programBlock << endl;
	work(programBlock, 0);
	return 0;
}
