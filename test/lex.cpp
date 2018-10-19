#include <stdio.h>
int yylex();
extern FILE *yyin;
int main()
{
    do  {
        printf(" %d\n", yylex());
    }while(!feof(yyin));

}
