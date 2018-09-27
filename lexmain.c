#include "lex.yy.c"
YYSTYPE yylval;
int main()
{
    int n = 10;
    while(n--) printf("%d \n", yylex());
}
