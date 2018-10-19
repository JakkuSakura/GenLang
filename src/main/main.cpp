#include <stdio.h>
void yyparse();
extern FILE *yyin;
int main()
{
    printf("%s\n", "running");
    yyparse();
    printf("%s\n", "end");
}
