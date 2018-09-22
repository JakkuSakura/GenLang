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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    NUMBER = 258,
    MOD = 259,
    RIGHTSHIFT = 260,
    LEFTSHIFT = 261,
    PIVAL = 262,
    PLUS = 263,
    MINUS = 264,
    DIV = 265,
    MUL = 266,
    POW = 267,
    SQRT = 268,
    OPENBRACKET = 269,
    CLOSEBRACKET = 270,
    UNARYMINUS = 271,
    ASIN = 272,
    ACOS = 273,
    ATAN = 274,
    SIN = 275,
    SINH = 276,
    COS = 277,
    COSH = 278,
    TAN = 279,
    TANH = 280,
    INC = 281,
    DEC = 282,
    LAND = 283,
    OR = 284,
    XOR = 285,
    ASSIGN = 286,
    IOR = 287,
    AND = 288,
    CEIL = 289,
    FLOOR = 290,
    ABS = 291,
    FACTORIAL = 292,
    BIN_DEC = 293,
    LOG = 294
  };
#endif
/* Tokens.  */
#define NUMBER 258
#define MOD 259
#define RIGHTSHIFT 260
#define LEFTSHIFT 261
#define PIVAL 262
#define PLUS 263
#define MINUS 264
#define DIV 265
#define MUL 266
#define POW 267
#define SQRT 268
#define OPENBRACKET 269
#define CLOSEBRACKET 270
#define UNARYMINUS 271
#define ASIN 272
#define ACOS 273
#define ATAN 274
#define SIN 275
#define SINH 276
#define COS 277
#define COSH 278
#define TAN 279
#define TANH 280
#define INC 281
#define DEC 282
#define LAND 283
#define OR 284
#define XOR 285
#define ASSIGN 286
#define IOR 287
#define AND 288
#define CEIL 289
#define FLOOR 290
#define ABS 291
#define FACTORIAL 292
#define BIN_DEC 293
#define LOG 294

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
