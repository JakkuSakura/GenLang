/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_SRC_GENLANG_PARSER_HH_INCLUDED
# define YY_YY_SRC_GENLANG_PARSER_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TIDENTIFIER = 258,
    TINTEGER = 259,
    TDOUBLE = 260,
    TVARTYPE = 261,
    TSTRING = 262,
    TCHAR = 263,
    TCEQ = 264,
    TCNE = 265,
    TCLT = 266,
    TCLE = 267,
    TCGT = 268,
    TCGE = 269,
    TEQUAL = 270,
    TENDSTATEMENT = 271,
    TLPAREN = 272,
    TRPAREN = 273,
    TLBRACE = 274,
    TRBRACE = 275,
    TCOMMA = 276,
    TDOT = 277,
    TPLUS = 278,
    TMINUS = 279,
    TMUL = 280,
    TDIV = 281,
    TRETURN = 282,
    TEXTERN = 283,
    TIMPORT = 284,
    TIF = 285,
    TFOR = 286,
    TWHILE = 287,
    TCONTINUE = 288,
    TBREAK = 289
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 13 "src/lexyacc/genlang.y" /* yacc.c:1909  */

	Node *node;
	NBlock *block;
	NExpression *expr;
	NStatement *stmt;
	NIdentifier *ident;
	NString *nstr;
	NChar *nchar;
	NVarType *vartype;
	NProgram *program;
	NVariableDeclaration *var_decl;
	std::vector<NVariableDeclaration*> *varvec;
	std::vector<NExpression*> *exprvec;
	std::string *string;
	int token;

#line 106 "src/genlang.parser.hh" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_GENLANG_PARSER_HH_INCLUDED  */
