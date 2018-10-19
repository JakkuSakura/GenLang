#include <iostream>
#include "node.h"

using namespace std;

extern int yyparse();
extern NBlock* programBlock;

int main(int argc, char **argv)
{
	yyparse();
	cout << programBlock << endl;
	return 0;
}
