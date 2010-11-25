
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "parser.y"

#include <stdio.h>
#include <string.h>
#include "../include/node.h"
#include "../include/stack.h"
#include "../include/query_parser.h"
#include "../include/macros.h"

#define LXQ_RELATION_TYPE 0
#define LXQ_SELECTOR_TYPE 1

extern int yylex(void);
extern int yyparse(void);
extern int yylineno;
extern char* yytext;
extern FILE* yyin;

doc* lxq_document;

char* lxq_parser_dot_query_operator = "class";
char* lxq_parser_pound_query_operator = "type";
list* lxq_selected_elements;

void yyerror(const char *str)
{
  fprintf(stderr,"error:%d: %s at '%s'\n", yylineno, str, yytext);
}

int yywrap()
{
        return 1;
}

void parse_file(char* filename){
  char* address;
  FILE* holder;
  struct yy_buffer_state* bs;

  holder = yyin = fopen(filename, "r");
  if(yyin == NULL){
    log(F, "Unnable to open file %s for reading\n", filename);
    exit(-1);
  }
  yylineno = 1;
  yyparse();
  yylex_destroy();
  fclose(holder);
}

void parse_string(const char* str){
  int len = strlen(str);
  char* internal_cpy = alloc(char, len + 2);
  struct yy_buffer_state* bs;

  memcpy(internal_cpy, str, len);

  internal_cpy[len] = '\0';
  internal_cpy[len + 1] = '\0';

  bs = (struct yy_buffer_state*) yy_scan_buffer(internal_cpy, len + 2);
  if(bs == NULL){
    log(F, "flex could not allocate a new buffer to parse the string.\n");
    exit(-1);
  }
  yylineno = 1;
  yyparse();
  yylex_destroy();
  free(internal_cpy);
}



/* Line 189 of yacc.c  */
#line 146 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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
     NO_OP = 267,
     EQUAL_OP = 268,
     WSSV_OP = 269,
     STARTSW_OP = 270,
     ENDSW_OP = 271,
     CONTAINS_OP = 272,
     REGEX_OP = 273,
     REGEXI_OP = 274,
     DSV_OP = 275,
     NOTEQUAL_OP = 276,
     EVEN = 277,
     ODD = 278,
     NTH_CHILD_FILTER = 279,
     NTH_LAST_CHILD_FILTER = 280,
     FIRST_CHILD_FILTER = 281,
     LAST_CHILD_FILTER = 282,
     ONLY_CHILD_FILTER = 283,
     EMPTY_FILTER = 284,
     NOT_FILTER = 285,
     DIGITS = 286
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
#define NO_OP 267
#define EQUAL_OP 268
#define WSSV_OP 269
#define STARTSW_OP 270
#define ENDSW_OP 271
#define CONTAINS_OP 272
#define REGEX_OP 273
#define REGEXI_OP 274
#define DSV_OP 275
#define NOTEQUAL_OP 276
#define EVEN 277
#define ODD 278
#define NTH_CHILD_FILTER 279
#define NTH_LAST_CHILD_FILTER 280
#define FIRST_CHILD_FILTER 281
#define LAST_CHILD_FILTER 282
#define ONLY_CHILD_FILTER 283
#define EMPTY_FILTER 284
#define NOT_FILTER 285
#define DIGITS 286




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 73 "parser.y"

  char * string;
  struct snode* dn;
  int digits;
  struct attr_selector_s* attrselector;
  struct step_s* s;
  int token;
  struct filter_selector_s* fa;
  struct generic_list_s* q;
  struct selector_s* sel;
 


/* Line 214 of yacc.c  */
#line 258 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 270 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   100

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  75
/* YYNRULES -- Number of states.  */
#define YYNSTATES  117

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   286

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    36,    40,     2,     2,     2,    49,
      41,    42,     2,    44,    48,    45,    39,    50,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    33,     2,
       2,    35,    46,    34,    32,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    37,     2,    38,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    51,     2,     2,     2,     2,
      43,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    47,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     8,    10,    13,    15,    19,    26,
      30,    36,    37,    40,    42,    44,    46,    51,    56,    57,
      60,    64,    68,    71,    73,    77,    81,    82,    87,    91,
      95,    98,    99,   101,   103,   104,   108,   110,   115,   120,
     122,   124,   126,   128,   130,   132,   134,   136,   138,   140,
     142,   145,   148,   152,   155,   158,   160,   162,   164,   166,
     168,   170,   171,   176,   181,   187,   189,   192,   193,   195,
     197,   199,   201,   203,   205,   207
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      53,     0,    -1,    32,    65,    -1,    54,    -1,    57,    -1,
      56,    57,    -1,     6,    -1,     6,    33,     6,    -1,     3,
      34,    55,    62,    34,     4,    -1,    60,    58,    61,    -1,
       3,    55,    62,     5,     4,    -1,    -1,    58,    59,    -1,
       8,    -1,     7,    -1,    57,    -1,     3,    55,    62,     4,
      -1,     3,     5,    55,     4,    -1,    -1,    62,    63,    -1,
      55,    35,    64,    -1,    36,     7,    36,    -1,    36,    36,
      -1,    66,    -1,    65,    77,    66,    -1,    69,    67,    70,
      -1,    -1,    67,    37,    68,    38,    -1,    67,    39,     6,
      -1,    67,    40,     6,    -1,     6,    78,    -1,    -1,    10,
      -1,     6,    -1,    -1,    70,    33,    71,    -1,    73,    -1,
      72,    41,    74,    42,    -1,    30,    41,    65,    42,    -1,
      24,    -1,    25,    -1,    26,    -1,    27,    -1,    28,    -1,
      29,    -1,    75,    -1,    76,    -1,    22,    -1,    23,    -1,
      43,    -1,    31,    43,    -1,    43,    76,    -1,    31,    43,
      76,    -1,    44,    31,    -1,    45,    31,    -1,    31,    -1,
      46,    -1,    47,    -1,    44,    -1,    48,    -1,    11,    -1,
      -1,    81,    36,     7,    36,    -1,    81,    49,     7,    49,
      -1,    13,    50,    79,    50,    80,    -1,     9,    -1,    79,
       9,    -1,    -1,    51,    -1,    13,    -1,    14,    -1,    15,
      -1,    16,    -1,    17,    -1,    20,    -1,    21,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   106,   106,   107,   110,   111,   114,   115,   122,   138,
     155,   166,   167,   172,   173,   174,   178,   189,   192,   193,
     199,   207,   208,   212,   213,   221,   224,   225,   226,   227,
     230,   233,   234,   235,   238,   239,   242,   243,   244,   247,
     248,   251,   252,   253,   254,   257,   258,   259,   260,   263,
     264,   265,   266,   269,   270,   271,   274,   275,   276,   277,
     278,   281,   282,   283,   284,   296,   297,   300,   301,   304,
     305,   306,   307,   308,   309,   310
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "START_EL", "END_EL", "SLASH", "WORD",
  "TEXT", "CDATA_TOK", "REGEX", "ALL", "SPACE", "NO_OP", "EQUAL_OP",
  "WSSV_OP", "STARTSW_OP", "ENDSW_OP", "CONTAINS_OP", "REGEX_OP",
  "REGEXI_OP", "DSV_OP", "NOTEQUAL_OP", "EVEN", "ODD", "NTH_CHILD_FILTER",
  "NTH_LAST_CHILD_FILTER", "FIRST_CHILD_FILTER", "LAST_CHILD_FILTER",
  "ONLY_CHILD_FILTER", "EMPTY_FILTER", "NOT_FILTER", "DIGITS", "'@'",
  "':'", "'?'", "'='", "'\"'", "'['", "']'", "'.'", "'#'", "'('", "')'",
  "'n'", "'+'", "'-'", "'>'", "'~'", "','", "'\\''", "'/'", "'i'",
  "$accept", "choose", "document", "namespace", "declaration", "node",
  "inner", "prop", "start_tag", "end_tag", "attrs", "attr", "value",
  "selector_group", "selector", "attrsels", "attrsel", "id",
  "pseudo_filters", "pseudo_filter", "nth_pseudo_op", "pseudo_op",
  "parameters", "step", "offset", "relation_operator", "attr_filter",
  "regex", "rmodifier", "operator", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,    64,    58,    63,    61,    34,    91,    93,    46,
      35,    40,    41,   110,    43,    45,    62,   126,    44,    39,
      47,   105
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    52,    53,    53,    54,    54,    55,    55,    56,    57,
      57,    58,    58,    59,    59,    59,    60,    61,    62,    62,
      63,    64,    64,    65,    65,    66,    67,    67,    67,    67,
      68,    69,    69,    69,    70,    70,    71,    71,    71,    72,
      72,    73,    73,    73,    73,    74,    74,    74,    74,    75,
      75,    75,    75,    76,    76,    76,    77,    77,    77,    77,
      77,    78,    78,    78,    78,    79,    79,    80,    80,    81,
      81,    81,    81,    81,    81,    81
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     2,     1,     3,     6,     3,
       5,     0,     2,     1,     1,     1,     4,     4,     0,     2,
       3,     3,     2,     1,     3,     3,     0,     4,     3,     3,
       2,     0,     1,     1,     0,     3,     1,     4,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     2,     2,     1,     1,     1,     1,     1,
       1,     0,     4,     4,     5,     1,     2,     0,     1,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    31,     0,     3,     0,     4,    11,     6,     0,
      18,    33,    32,     2,    23,    26,     1,     0,     5,     0,
       0,    18,     0,    60,    58,    56,    57,    59,    31,    34,
       0,    14,    13,    15,    12,     9,     7,     0,    16,     0,
       0,    19,    24,     0,     0,     0,    25,     0,     0,    10,
       0,    61,     0,    28,    29,     0,     0,     8,     0,    20,
      69,    70,    71,    72,    73,    74,    75,    30,     0,    27,
      39,    40,    41,    42,    43,    44,     0,    35,     0,    36,
      17,     0,    22,     0,     0,     0,    31,     0,    21,    65,
       0,     0,     0,     0,    47,    48,    55,    49,     0,     0,
       0,    45,    46,    66,    67,    62,    63,    38,    50,    55,
      51,    53,    54,    37,    68,    64,    52
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,    10,     5,     6,    19,    34,     7,    35,
      22,    41,    59,    13,    14,    29,    52,    15,    46,    77,
      78,    79,   100,   101,   102,    28,    67,    90,   115,    68
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -88
static const yytype_int8 yypact[] =
{
      -1,     0,    16,    11,   -88,    10,   -88,   -88,    13,    18,
     -88,   -88,   -88,     1,   -88,   -88,   -88,    18,   -88,    12,
      24,   -88,    70,   -88,   -88,   -88,   -88,   -88,    16,   -12,
      64,   -88,   -88,   -88,   -88,   -88,   -88,     2,   -88,    31,
      17,   -88,   -88,    26,    52,    71,    45,    18,    33,   -88,
      43,    40,    42,   -88,   -88,    38,    77,   -88,    -3,   -88,
      32,   -88,   -88,   -88,   -88,   -88,   -88,   -88,   -31,   -88,
     -88,   -88,   -88,   -88,   -88,   -88,    44,   -88,    46,   -88,
     -88,    47,   -88,    75,    79,    81,    16,    28,   -88,   -88,
      -9,    53,    41,    -4,   -88,   -88,    48,   -28,    61,    62,
      54,   -88,   -88,   -88,    49,   -88,   -88,   -88,   -28,   -88,
     -88,   -88,   -88,   -88,   -88,   -88,   -88
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -88,   -88,   -88,    -8,   -88,     4,   -88,   -88,   -88,   -88,
      73,   -88,   -88,     9,    69,   -88,   -88,   -88,   -88,   -88,
     -88,   -88,   -88,   -88,   -87,   -88,   -88,   -88,   -88,   -88
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
     103,    21,     1,   109,    81,    84,     8,    23,     8,    18,
     110,    16,    23,    17,    40,    30,    98,    99,    85,    31,
      32,   116,    11,    33,     8,    43,    12,    44,    45,    40,
      36,     2,    51,    82,     9,    49,    48,    57,   107,    56,
      24,   104,    25,    26,    27,    24,    20,    25,    26,    27,
      94,    95,    50,    60,    61,    62,    63,    64,    53,    96,
      65,    66,    70,    71,    72,    73,    74,    75,    76,    47,
       8,    97,    98,    99,    38,    39,     8,    54,    55,    58,
      69,    80,    83,    88,    89,    86,    91,    87,    92,   105,
     106,   108,   111,   112,    37,    93,   113,    42,     0,     0,
     114
};

static const yytype_int8 yycheck[] =
{
       9,     9,     3,    31,     7,    36,     6,    11,     6,     5,
      97,     0,    11,     3,    22,     3,    44,    45,    49,     7,
       8,   108,     6,    19,     6,    37,    10,    39,    40,    37,
       6,    32,     6,    36,    34,     4,    34,     4,    42,    47,
      44,    50,    46,    47,    48,    44,    33,    46,    47,    48,
      22,    23,    35,    13,    14,    15,    16,    17,     6,    31,
      20,    21,    24,    25,    26,    27,    28,    29,    30,     5,
       6,    43,    44,    45,     4,     5,     6,     6,    33,    36,
      38,     4,    50,    36,     9,    41,     7,    41,     7,    36,
      49,    43,    31,    31,    21,    86,    42,    28,    -1,    -1,
      51
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    32,    53,    54,    56,    57,    60,     6,    34,
      55,     6,    10,    65,    66,    69,     0,     3,    57,    58,
      33,    55,    62,    11,    44,    46,    47,    48,    77,    67,
       3,     7,     8,    57,    59,    61,     6,    62,     4,     5,
      55,    63,    66,    37,    39,    40,    70,     5,    34,     4,
      35,     6,    68,     6,     6,    33,    55,     4,    36,    64,
      13,    14,    15,    16,    17,    20,    21,    78,    81,    38,
      24,    25,    26,    27,    28,    29,    30,    71,    72,    73,
       4,     7,    36,    50,    36,    49,    41,    41,    36,     9,
      79,     7,     7,    65,    22,    23,    31,    43,    44,    45,
      74,    75,    76,     9,    50,    36,    49,    42,    43,    31,
      76,    31,    31,    42,    51,    80,    76
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:

/* Line 1455 of yacc.c  */
#line 110 "parser.y"
    {lxq_document = new_document(NULL); set_doc_root(lxq_document, (yyvsp[(1) - (1)].dn));}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 111 "parser.y"
    {lxq_document = new_document((yyvsp[(1) - (2)].dn)); set_doc_root(lxq_document, (yyvsp[(2) - (2)].dn));}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 114 "parser.y"
    { (yyval.dn) = new_element_node((yyvsp[(1) - (1)].string));}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 115 "parser.y"
    { (yyval.dn) = new_element_node((yyvsp[(3) - (3)].string)); 
                                                              char* old = set_namespace((yyval.dn), (yyvsp[(1) - (3)].string));
							      if(old)
								free(old);
	                                                    }
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 122 "parser.y"
    {
                                                              if(strcmp(get_name((yyvsp[(3) - (6)].dn)), "xml") != 0){
                                                                yyerror("Declaration does not begin with xml");
                                                                exit(-1);
                                                              }
                                                              (yyval.dn) = (yyvsp[(4) - (6)].dn);
                                                              char* old = set_name((yyval.dn), get_name((yyvsp[(3) - (6)].dn)));
							      if(old)
								free(old);
                                                              old = set_namespace((yyval.dn), get_namespace((yyvsp[(3) - (6)].dn)));
							      if(old)
								free(old);
                                                              destroy_dom_node((yyvsp[(3) - (6)].dn));
                                                            }
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 138 "parser.y"
    {
                                                              if(strcmp(get_name((yyvsp[(1) - (3)].dn)),get_name((yyvsp[(3) - (3)].dn))) != 0)
                                                              {
								char error_line[1024] = {0};
								sprintf(error_line, "Start tag '%s' does not match end tag '%s' ", get_name((yyvsp[(1) - (3)].dn)), get_name((yyvsp[(3) - (3)].dn)));
                                                                yyerror(error_line);
                                                                exit(1);
                                                              }
                                                              append_children((yyvsp[(1) - (3)].dn), (yyvsp[(2) - (3)].dn)->children);
							      if((yyvsp[(2) - (3)].dn)->namespace != NULL)
								free((yyvsp[(2) - (3)].dn)->namespace);
                                                              free((yyvsp[(2) - (3)].dn)->name);
                                                              destroy_generic_list((yyvsp[(2) - (3)].dn)->children);
                                                              free((yyvsp[(2) - (3)].dn));
                                                              (yyval.dn) = (yyvsp[(1) - (3)].dn);
							      destroy_dom_node((yyvsp[(3) - (3)].dn));
                                                            }
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 155 "parser.y"
    { (yyval.dn) = (yyvsp[(3) - (5)].dn);
                                                              char* old = set_name((yyval.dn), get_name((yyvsp[(2) - (5)].dn)));
							      if(old)
								free(old);							    
                                                              old = set_namespace((yyval.dn), get_namespace((yyvsp[(2) - (5)].dn)));
							      if(old)
								free(old);
                                                              destroy_dom_node((yyvsp[(2) - (5)].dn));
                                                            }
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 166 "parser.y"
    { (yyval.dn) = new_element_node("");}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 167 "parser.y"
    { (yyval.dn) = (yyvsp[(1) - (2)].dn);
                                                              append_child((yyval.dn), (yyvsp[(2) - (2)].dn));
                                                            }
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 172 "parser.y"
    {(yyval.dn) = new_cdata((yyvsp[(1) - (1)].string));}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 173 "parser.y"
    {(yyval.dn) = new_text_node((yyvsp[(1) - (1)].string)); }
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 174 "parser.y"
    {(yyval.dn) = (yyvsp[(1) - (1)].dn);}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 178 "parser.y"
    { (yyval.dn) = (yyvsp[(3) - (4)].dn);
                                                              char* old = set_name((yyval.dn), get_name((yyvsp[(2) - (4)].dn)));
							      if(old)
								free(old);
                                                              old = set_namespace((yyval.dn), get_namespace((yyvsp[(2) - (4)].dn)));
							      if(old)
								free(old);
                                                              destroy_dom_node((yyvsp[(2) - (4)].dn));
                                                            }
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 189 "parser.y"
    { (yyval.dn) = (yyvsp[(3) - (4)].dn);}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 192 "parser.y"
    { (yyval.dn) = new_element_node(""); }
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 193 "parser.y"
    {
                                                              (yyval.dn) = (yyvsp[(1) - (2)].dn);
                                                              add_attribute((yyval.dn), (yyvsp[(2) - (2)].dn));
                                                            }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 199 "parser.y"
    {(yyval.dn) = new_attribute(get_name((yyvsp[(1) - (3)].dn)), (yyvsp[(3) - (3)].string)); 
                                                             char* old = set_namespace((yyval.dn), get_namespace((yyvsp[(1) - (3)].dn))); 
							     if(old)
							       free(old);
							     destroy_dom_node((yyvsp[(1) - (3)].dn)); }
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 207 "parser.y"
    {(yyval.string) = (yyvsp[(2) - (3)].string);}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 208 "parser.y"
    {(yyval.string) = "";}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 212 "parser.y"
    { lxq_selected_elements = (yyval.q) = new_queue(16); enqueue_with_type((yyval.q), (yyvsp[(1) - (1)].sel), LXQ_SELECTOR_TYPE); }
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 213 "parser.y"
    { int* a = alloc(int, 1);
                                                              *a = (yyvsp[(2) - (3)].token);
                                                              (yyval.q) = (yyvsp[(1) - (3)].q);
                                                              enqueue_with_type((yyval.q), a, LXQ_RELATION_TYPE);
                                                              enqueue_with_type((yyval.q), (yyvsp[(3) - (3)].sel), LXQ_SELECTOR_TYPE);
                                                            }
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 221 "parser.y"
    { (yyval.sel) = new_selector((yyvsp[(1) - (3)].string)); (yyval.sel)->attrs = (yyvsp[(2) - (3)].q); (yyval.sel)->filters = (yyvsp[(3) - (3)].q); }
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 224 "parser.y"
    { (yyval.q) = new_stack(4); }
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 225 "parser.y"
    { (yyval.q) = (yyvsp[(1) - (4)].q); push_stack((yyval.q), (yyvsp[(3) - (4)].attrselector)); }
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 226 "parser.y"
    { (yyval.q) = (yyvsp[(1) - (3)].q); push_stack((yyval.q), new_attr_value_selector(lxq_parser_dot_query_operator, EQUAL_OP, (yyvsp[(3) - (3)].string))); }
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 227 "parser.y"
    { (yyval.q) = (yyvsp[(1) - (3)].q); push_stack((yyval.q), new_attr_value_selector(lxq_parser_pound_query_operator, EQUAL_OP, (yyvsp[(3) - (3)].string))); }
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 230 "parser.y"
    { (yyval.attrselector) = (yyvsp[(2) - (2)].attrselector); (yyval.attrselector)->name = strdup((yyvsp[(1) - (2)].string)); }
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 233 "parser.y"
    { (yyval.string) = NULL; }
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 234 "parser.y"
    { (yyval.string) = NULL; }
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 235 "parser.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string);}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 238 "parser.y"
    { (yyval.q) = new_stack(4); }
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 239 "parser.y"
    { push_stack((yyval.q), (yyvsp[(3) - (3)].fa)); }
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 242 "parser.y"
    { (yyval.fa) = new_filter((yyvsp[(1) - (1)].token)); }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 243 "parser.y"
    { (yyval.fa) = new_filter((yyvsp[(1) - (4)].token)); (yyval.fa)->value.s = (yyvsp[(3) - (4)].s); }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 244 "parser.y"
    { (yyval.fa) = new_filter(NOT_FILTER); (yyval.fa)->value.selector = (yyvsp[(3) - (4)].q); }
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 247 "parser.y"
    { (yyval.token) = NTH_CHILD_FILTER; }
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 248 "parser.y"
    { (yyval.token) = NTH_LAST_CHILD_FILTER; }
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 251 "parser.y"
    { (yyval.token) = FIRST_CHILD_FILTER; }
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 252 "parser.y"
    { (yyval.token) = LAST_CHILD_FILTER; }
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 253 "parser.y"
    { (yyval.token) = ONLY_CHILD_FILTER; }
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 254 "parser.y"
    { (yyval.token) = EMPTY_FILTER; }
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 257 "parser.y"
    { (yyval.s) = (yyvsp[(1) - (1)].s); }
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 258 "parser.y"
    { (yyval.s) = alloc(struct step_s, 1); (yyval.s)->multiplier = 0; (yyval.s)->offset = (yyvsp[(1) - (1)].digits);}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 259 "parser.y"
    { (yyval.s) = alloc(struct step_s, 1); (yyval.s)->multiplier = 2; (yyval.s)->offset = 0; }
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 260 "parser.y"
    { (yyval.s) = alloc(struct step_s, 1); (yyval.s)->multiplier = 2; (yyval.s)->offset = 1; }
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 263 "parser.y"
    { (yyval.s) = alloc(struct step_s, 1); (yyval.s)->multiplier = 1; (yyval.s)->offset = 0; }
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 264 "parser.y"
    { (yyval.s) = alloc(struct step_s, 1); (yyval.s)->multiplier = (yyvsp[(1) - (2)].digits); (yyval.s)->offset = 0; }
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 265 "parser.y"
    { (yyval.s) = alloc(struct step_s, 1); (yyval.s)->multiplier = 1; (yyval.s)->offset = (yyvsp[(2) - (2)].digits); }
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 266 "parser.y"
    { (yyval.s) = alloc(struct step_s, 1); (yyval.s)->multiplier = (yyvsp[(1) - (3)].digits); (yyval.s)->offset = (yyvsp[(3) - (3)].digits); }
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 269 "parser.y"
    { (yyval.digits) = (yyvsp[(2) - (2)].digits); }
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 270 "parser.y"
    { (yyval.digits) = -(yyvsp[(2) - (2)].digits); }
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 271 "parser.y"
    { (yyval.digits) = (yyvsp[(1) - (1)].digits); }
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 274 "parser.y"
    { (yyval.token) = '>'; }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 275 "parser.y"
    { (yyval.token) = '~'; }
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 276 "parser.y"
    { (yyval.token) = '+'; }
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 277 "parser.y"
    { (yyval.token) = ','; }
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 278 "parser.y"
    { (yyval.token) = SPACE; }
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 281 "parser.y"
    { (yyval.attrselector) = new_attr_value_selector(NULL, NO_OP, NULL); }
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 282 "parser.y"
    { (yyval.attrselector) = new_attr_value_selector(NULL, (yyvsp[(1) - (4)].token), (yyvsp[(3) - (4)].string)); }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 283 "parser.y"
    { (yyval.attrselector) = new_attr_value_selector(NULL, (yyvsp[(1) - (4)].token), (yyvsp[(3) - (4)].string)); }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 284 "parser.y"
    {
                                                                char* text = (char*)pop_stack((yyvsp[(3) - (5)].q));
                                                                while((yyvsp[(3) - (5)].q)->count > 0){
                                                                    char* r = (char*)pop_stack((yyvsp[(3) - (5)].q));
                                                                    text = (char*)realloc(text, strlen(text) + strlen(r) + 1);
                                                                    strcat(text, r);
                                                                }
                                                                (yyval.attrselector) = new_attr_value_selector(NULL, (yyvsp[(5) - (5)].token), text);
                                                                destroy_generic_list((yyvsp[(3) - (5)].q));
                                                            }
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 296 "parser.y"
    { (yyval.q) = new_stack(4); push_stack((yyval.q), (yyvsp[(1) - (1)].string)); }
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 297 "parser.y"
    { (yyval.q) = (yyvsp[(1) - (2)].q); push_stack((yyval.q), (yyvsp[(2) - (2)].string)); }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 300 "parser.y"
    { (yyval.token) = REGEX_OP; }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 301 "parser.y"
    { (yyval.token) = REGEXI_OP; }
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 304 "parser.y"
    { (yyval.token) = EQUAL_OP; }
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 305 "parser.y"
    { (yyval.token) = WSSV_OP; }
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 306 "parser.y"
    { (yyval.token) = STARTSW_OP; }
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 307 "parser.y"
    { (yyval.token) = ENDSW_OP; }
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 308 "parser.y"
    { (yyval.token) = CONTAINS_OP; }
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 309 "parser.y"
    { (yyval.token) = DSV_OP; }
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 310 "parser.y"
    { (yyval.token) = NOTEQUAL_OP; }
    break;



/* Line 1455 of yacc.c  */
#line 2148 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



