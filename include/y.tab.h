
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     START_EL = 258,
     END_EL = 259,
     SLASH = 260,
     WORD = 261,
     TEXT = 262,
     CDATA_TOK = 263,
     REGEX = 264,
     ALL = 265,
     SPACE = 266,
     END_REGEXI = 267,
     NO_OP = 268,
     EQUAL_OP = 269,
     WSSV_OP = 270,
     STARTSW_OP = 271,
     ENDSW_OP = 272,
     CONTAINS_OP = 273,
     REGEX_OP = 274,
     REGEXI_OP = 275,
     DSV_OP = 276,
     NOTEQUAL_OP = 277,
     EVEN = 278,
     ODD = 279,
     NTH_CHILD_FILTER = 280,
     NTH_LAST_CHILD_FILTER = 281,
     FIRST_CHILD_FILTER = 282,
     LAST_CHILD_FILTER = 283,
     ONLY_CHILD_FILTER = 284,
     EMPTY_FILTER = 285,
     NOT_FILTER = 286,
     DIGITS = 287
   };
#endif
/* Tokens.  */
#define START_EL 258
#define END_EL 259
#define SLASH 260
#define WORD 261
#define TEXT 262
#define CDATA_TOK 263
#define REGEX 264
#define ALL 265
#define SPACE 266
#define END_REGEXI 267
#define NO_OP 268
#define EQUAL_OP 269
#define WSSV_OP 270
#define STARTSW_OP 271
#define ENDSW_OP 272
#define CONTAINS_OP 273
#define REGEX_OP 274
#define REGEXI_OP 275
#define DSV_OP 276
#define NOTEQUAL_OP 277
#define EVEN 278
#define ODD 279
#define NTH_CHILD_FILTER 280
#define NTH_LAST_CHILD_FILTER 281
#define FIRST_CHILD_FILTER 282
#define LAST_CHILD_FILTER 283
#define ONLY_CHILD_FILTER 284
#define EMPTY_FILTER 285
#define NOT_FILTER 286
#define DIGITS 287




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 71 "parser.y"

  char * string;
  struct snode* dn;
  int digits;
  struct attr_selector_s* attrselector;
  struct step_s* s;
  int token;
  struct filter_selector_s* fa;
  struct generic_list_s* q;
  struct selector_s* sel;
 


/* Line 1676 of yacc.c  */
#line 130 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


