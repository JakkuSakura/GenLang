D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
DQUA		\"
%{
#include <stdlib.h>
#include "Node.h"
#include "genlang.parser.hh"
void count();
int yylex();
void comment();
int check_type();
void raw_string();
void yyerror(const char *);
extern YYSTYPE yylval;
%}
%%
"/*".*?"*/"		{ /*comment*/  }

"auto"			{ count(); yylval = new NReserved(AUTO); return AUTO; }
"break"			{ count(); yylval = new NReserved(BREAK); return BREAK; }
"case"			{ count(); yylval = new NReserved(CASE); return CASE; }
"char"			{ count(); yylval = new NReserved(CHAR); return CHAR; }
"const"			{ count(); yylval = new NReserved(CONST); return CONST; }
"continue"		{ count(); yylval = new NReserved(CONTINUE); return CONTINUE; }
"default"		{ count(); yylval = new NReserved(DEFAULT); return DEFAULT; }
"do"			{ count(); yylval = new NReserved(DO); return DO; }
"double"		{ count(); yylval = new NReserved(DOUBLE); return DOUBLE; }
"else"			{ count(); yylval = new NReserved(ELSE); return ELSE; }
"enum"			{ count(); yylval = new NReserved(ENUM); return ENUM; }
"extern"		{ count(); yylval = new NReserved(EXTERN); return EXTERN; }
"float"			{ count(); yylval = new NReserved(FLOAT); return FLOAT; }
"for"			{ count(); yylval = new NReserved(FOR); return FOR; }
"goto"			{ count(); yylval = new NReserved(GOTO); return GOTO; }
"if"			{ count(); yylval = new NReserved(IF); return IF; }
"int"			{ count(); yylval = new NReserved(INT); return INT; }
"long"			{ count(); yylval = new NReserved(LONG); return LONG; }
"register"		{ count(); yylval = new NReserved(REGISTER); return REGISTER; }
"return"		{ count(); yylval = new NReserved(RETURN); return RETURN; }
"short"			{ count(); yylval = new NReserved(SHORT); return SHORT; }
"signed"		{ count(); yylval = new NReserved(SIGNED); return SIGNED; }
"sizeof"		{ count(); yylval = new NReserved(SIZEOF); return SIZEOF; }
"static"		{ count(); yylval = new NReserved(STATIC); return STATIC; }
"struct"		{ count(); yylval = new NReserved(STRUCT); return STRUCT; }
"switch"		{ count(); yylval = new NReserved(SWITCH); return SWITCH; }
"typedef"		{ count(); yylval = new NReserved(TYPEDEF); return TYPEDEF; }
"union"			{ count(); yylval = new NReserved(UNION); return UNION; }
"unsigned"		{ count(); yylval = new NReserved(UNSIGNED); return UNSIGNED; }
"void"			{ count(); yylval = new NReserved(VOID); return VOID; }
"volatile"		{ count(); yylval = new NReserved(VOLATILE); return VOLATILE; }
"while"			{ count(); yylval = new NReserved(WHILE); return WHILE; }
{L}({L}|{D})*		{ count(); yylval = new NIdentifier(yytext); return(check_type()); } /* TODO: to complete this function */

0[xX]{H}+		{
	 				count();
					yylval = new NInteger(strtoul(yytext, NULL, 0), 4, 0);
	  				return(CONSTANT);
  				}

{D}+			{
	 				count();
					yylval = new NInteger(strtoul(yytext, NULL, 0), 4, 0);
	  				return(CONSTANT);
				}

{D}+{E}?			{ count(); yylval = new NDouble(strtod(yytext, NULL)); return(CONSTANT); }
{D}*"."{D}+({E})?	{ count(); yylval = new NDouble(strtod(yytext, NULL)); return(CONSTANT); }
{D}+"."{D}*({E})?	{ count(); yylval = new NDouble(strtod(yytext, NULL)); return(CONSTANT); }

{DQUA} {
					count();
					char *s = yytext;
					//raw_string();
					yylval = new NString(s + 1);
					((NString *)yylval)->value.pop_back();
					fprintf(stderr, "read %s", s);
					return(STRING_LITERAL);
				}

">>="			{ count(); yylval = new NBinaryOperator(RIGHT_ASSIGN); return RIGHT_ASSIGN; }
"<<="			{ count(); yylval = new NBinaryOperator(LEFT_ASSIGN); return LEFT_ASSIGN; }
"+="			{ count(); yylval = new NBinaryOperator(ADD_ASSIGN); return ADD_ASSIGN; }
"-="			{ count(); yylval = new NBinaryOperator(SUB_ASSIGN); return SUB_ASSIGN; }
"*="			{ count(); yylval = new NBinaryOperator(MUL_ASSIGN); return MUL_ASSIGN; }
"/="			{ count(); yylval = new NBinaryOperator(DIV_ASSIGN); return DIV_ASSIGN; }
"%="			{ count(); yylval = new NBinaryOperator(MOD_ASSIGN); return MOD_ASSIGN; }
"&="			{ count(); yylval = new NBinaryOperator(AND_ASSIGN); return AND_ASSIGN; }
"^="			{ count(); yylval = new NBinaryOperator(XOR_ASSIGN); return XOR_ASSIGN; }
"|="			{ count(); yylval = new NBinaryOperator(OR_ASSIGN); return OR_ASSIGN; }
">>"			{ count(); yylval = new NBinaryOperator(RIGHT_OP); return RIGHT_OP; }
"<<"			{ count(); yylval = new NBinaryOperator(LEFT_OP); return LEFT_OP; }
"++"			{ count(); yylval = new NBinaryOperator(INC_OP); return INC_OP; }
"--"			{ count(); yylval = new NBinaryOperator(DEC_OP); return DEC_OP; }
"->"			{ count(); yylval = new NBinaryOperator(PTR_OP); return PTR_OP; }
"&&"			{ count(); yylval = new NBinaryOperator(AND_OP); return AND_OP; }
"||"			{ count(); yylval = new NBinaryOperator(OR_OP); return OR_OP; }
"<="			{ count(); yylval = new NBinaryOperator(LE_OP); return LE_OP; }
">="			{ count(); yylval = new NBinaryOperator(GE_OP); return GE_OP; }
"=="			{ count(); yylval = new NBinaryOperator(EQ_OP); return EQ_OP; }
"!="			{ count(); yylval = new NBinaryOperator(NE_OP); return NE_OP; }
";"				{ count(); return(';'); }
"{"				{ count(); return('{'); }
"}"				{ count(); return('}'); }
","				{ count(); yylval = new NBinaryOperator(','); return(','); }
"="				{ count(); yylval = new NBinaryOperator('='); return('='); }
"("				{ count(); return('('); }
")"				{ count(); return(')'); }
"["				{ count(); return('['); }
"]"				{ count(); return(']'); }
"."				{ count(); yylval = new NBinaryOperator('.'); return('.'); }
"&"				{ count(); yylval = new NBinaryOperator('&'); return('&'); }
"!"				{ count(); yylval = new NSingleOperator('!'); return('!'); }
"~"				{ count(); yylval = new NSingleOperator('~'); return('~'); }
"-"				{ count(); return('-'); } /* + and - must be processed in yacc*/
"+"				{ count(); return('+'); }
"*"				{ count(); yylval = new NBinaryOperator('*'); return('*'); }
"/"				{ count(); yylval = new NBinaryOperator('/'); return('/'); }
"%"				{ count(); yylval = new NBinaryOperator('%'); return('%'); }
"<"				{ count(); yylval = new NBinaryOperator('<'); return('<'); }
">"				{ count(); yylval = new NBinaryOperator('>'); return('>'); }
"^"				{ count(); yylval = new NBinaryOperator('^'); return('^'); }
"|"				{ count(); yylval = new NBinaryOperator('|'); return('|'); }
"?"				{ count(); return('?'); } /* This will be translated into if-else statement*/
":"				{ count(); return(':'); }

[ \t\v\n\f]		{ count(); }
.				{ fprintf(stderr, "error %d", yytext[0]); /* ignore bad characters */ }

%%
#include <stdio.h>

int yywrap()
{
	return 1;
}

void raw_string()
{
	static const char *trans = "\"\n\r\\\t";
	char c;
	bool splash = false;
	while((c = yyinput()) != '"' || splash)
	{
		if (splash)
		{
			const char *p = trans;
			while(*p)
			{
				if(c == *p)
				{
					putchar(*p);
					goto end;
				}
			}
			count();
			char buf[64];
			sprintf(buf, "Unexcepted token %c %d after '\\'", c, (int)c);
			yyerror(buf);
			end:
			splash = false;
		}
		else if(c == '\\')
		{
			splash = true;
		}
		else
		{
			putchar(c);
		}
	}
}


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

	ECHO;
}


int check_type()
{
/*
* pseudo code --- this is what it should check
*
*	if (yytext == type_name)
*		return(TYPE_NAME);
*
*	return(IDENTIFIER);
*/

/*
*	it actually will only return IDENTIFIER
*/

	return(IDENTIFIER);
}
