
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
     SCUSTOM_FILTER = 261,
     WORD = 262,
     TEXT = 263,
     CDATA_TOK = 264,
     REGEX = 265,
     CUSTOM_FILTER = 266,
     ALL = 267,
     SPACE = 268,
     END_REGEXI = 269,
     NO_OP = 270,
     EQUAL_OP = 271,
     WSSV_OP = 272,
     STARTSW_OP = 273,
     ENDSW_OP = 274,
     CONTAINS_OP = 275,
     REGEX_OP = 276,
     REGEXI_OP = 277,
     DSV_OP = 278,
     NOTEQUAL_OP = 279,
     EVEN = 280,
     ODD = 281,
     NTH_CHILD_FILTER = 282,
     NTH_LAST_CHILD_FILTER = 283,
     FIRST_CHILD_FILTER = 284,
     LAST_CHILD_FILTER = 285,
     ONLY_CHILD_FILTER = 286,
     EMPTY_FILTER = 287,
     NOT_FILTER = 288,
     DIGITS = 289
   };
#endif
/* Tokens.  */
#define START_EL 258
#define END_EL 259
#define SLASH 260
#define SCUSTOM_FILTER 261
#define WORD 262
#define TEXT 263
#define CDATA_TOK 264
#define REGEX 265
#define CUSTOM_FILTER 266
#define ALL 267
#define SPACE 268
#define END_REGEXI 269
#define NO_OP 270
#define EQUAL_OP 271
#define WSSV_OP 272
#define STARTSW_OP 273
#define ENDSW_OP 274
#define CONTAINS_OP 275
#define REGEX_OP 276
#define REGEXI_OP 277
#define DSV_OP 278
#define NOTEQUAL_OP 279
#define EVEN 280
#define ODD 281
#define NTH_CHILD_FILTER 282
#define NTH_LAST_CHILD_FILTER 283
#define FIRST_CHILD_FILTER 284
#define LAST_CHILD_FILTER 285
#define ONLY_CHILD_FILTER 286
#define EMPTY_FILTER 287
#define NOT_FILTER 288
#define DIGITS 289




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 96 "parser.y"

  char * string;
  struct snode* dn;
  int digits;
  struct attr_selector_s* attrselector;
  struct step_s* s;
  int token;
  struct filter_selector_s* fa;
  struct generic_list_s* q;
  struct selector_s* sel;
  struct match_value_s* mv;
 


/* Line 1676 of yacc.c  */
#line 135 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


