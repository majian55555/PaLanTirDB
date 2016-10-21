/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     RW_CREATE = 258,
     RW_BUILD = 259,
     RW_REBUILD = 260,
     RW_DROP = 261,
     RW_DESTROY = 262,
     RW_PRINT = 263,
     RW_LOAD = 264,
     RW_HELP = 265,
     RW_QUIT = 266,
     RW_SELECT = 267,
     RW_INTO = 268,
     RW_WHERE = 269,
     RW_INSERT = 270,
     RW_DELETE = 271,
     RW_PRIMARY = 272,
     RW_NUMBUCKETS = 273,
     RW_ALL = 274,
     RW_FROM = 275,
     RW_SET = 276,
     RW_AS = 277,
     RW_TABLE = 278,
     RW_AND = 279,
     RW_OR = 280,
     RW_NOT = 281,
     RW_UPDATE = 282,
     RW_VALUES = 283,
     RW_DATABASE = 284,
     RW_OPEN = 285,
     RW_CLOSE = 286,
     INT_TYPE = 287,
     REAL_TYPE = 288,
     CHAR_TYPE = 289,
     VCHAR_TYPE = 290,
     DATE_TYPE = 291,
     DATETIME_TYPE = 292,
     BOOLEAN_TYPE = 293,
     BLOB_TYPE = 294,
     CLOB_TYPE = 295,
     T_EQ = 296,
     T_LT = 297,
     T_LE = 298,
     T_GT = 299,
     T_GE = 300,
     T_NE = 301,
     T_EOF = 302,
     T_ISNULL = 303,
     T_ISNOTNULL = 304,
     T_NULL = 305,
     NOTOKEN = 306,
     T_INT = 307,
     T_REAL = 308,
     T_STRING = 309,
     T_QSTRING = 310,
     T_SHELL_CMD = 311
   };
#endif
/* Tokens.  */
#define RW_CREATE 258
#define RW_BUILD 259
#define RW_REBUILD 260
#define RW_DROP 261
#define RW_DESTROY 262
#define RW_PRINT 263
#define RW_LOAD 264
#define RW_HELP 265
#define RW_QUIT 266
#define RW_SELECT 267
#define RW_INTO 268
#define RW_WHERE 269
#define RW_INSERT 270
#define RW_DELETE 271
#define RW_PRIMARY 272
#define RW_NUMBUCKETS 273
#define RW_ALL 274
#define RW_FROM 275
#define RW_SET 276
#define RW_AS 277
#define RW_TABLE 278
#define RW_AND 279
#define RW_OR 280
#define RW_NOT 281
#define RW_UPDATE 282
#define RW_VALUES 283
#define RW_DATABASE 284
#define RW_OPEN 285
#define RW_CLOSE 286
#define INT_TYPE 287
#define REAL_TYPE 288
#define CHAR_TYPE 289
#define VCHAR_TYPE 290
#define DATE_TYPE 291
#define DATETIME_TYPE 292
#define BOOLEAN_TYPE 293
#define BLOB_TYPE 294
#define CLOB_TYPE 295
#define T_EQ 296
#define T_LT 297
#define T_LE 298
#define T_GT 299
#define T_GE 300
#define T_NE 301
#define T_EOF 302
#define T_ISNULL 303
#define T_ISNOTNULL 304
#define T_NULL 305
#define NOTOKEN 306
#define T_INT 307
#define T_REAL 308
#define T_STRING 309
#define T_QSTRING 310
#define T_SHELL_CMD 311

#include "parse.h"


/* Copy the first part of user declarations.  */
#line 1 "parse.ypp"

//
// parser.y: yacc specification for simple SQL-like query language
//

#include <stdlib.h>
#include "RelManager.h"
#include "parse.tab.h"


extern "C" int isatty(int);
extern int yylex();
extern int yywrap();
extern void reset_scanner();
extern void quit();

void yyerror(char *);

extern char *yytext;                    // tokens in string format
static NODE *parse_tree;                // root of parse tree
extern int yylineno;


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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 24 "parse.ypp"
typedef union YYSTYPE {
  int ival;
  float rval;
  char *sval;
  NODE *n;
} YYSTYPE;
/* Line 196 of yacc.c.  */
#line 227 "parse.tab.cpp"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 239 "parse.tab.cpp"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

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

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
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
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  60
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   185

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  43
/* YYNRULES -- Number of rules. */
#define YYNRULES  98
/* YYNRULES -- Number of states. */
#define YYNSTATES  191

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   311

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      58,    59,    62,     2,    60,     2,    61,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    57,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     6,     8,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      40,    42,    44,    46,    53,    57,    61,    63,    66,    68,
      79,    83,    85,    87,    91,    93,    95,   100,   106,   114,
     118,   122,   126,   130,   134,   140,   146,   149,   157,   161,
     164,   167,   173,   175,   178,   180,   183,   185,   188,   190,
     192,   194,   198,   202,   206,   210,   212,   216,   220,   222,
     224,   228,   230,   234,   238,   240,   247,   254,   258,   262,
     266,   270,   274,   278,   282,   284,   287,   289,   291,   293,
     295,   297,   299,   301,   303,   305,   307,   309,   311
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      64,     0,    -1,    65,    57,    -1,    56,    -1,     1,    -1,
      47,    -1,    66,    -1,    69,    -1,    74,    -1,    75,    -1,
      76,    -1,    77,    -1,    78,    -1,    79,    -1,    80,    -1,
      81,    -1,    82,    -1,    83,    -1,    84,    -1,    85,    -1,
      86,    -1,    87,    -1,   105,    -1,    12,    95,    89,    20,
      67,    91,    -1,    58,    67,    59,    -1,    68,    60,    67,
      -1,    68,    -1,   104,   104,    -1,   104,    -1,    15,    13,
     104,    58,    70,    59,    28,    58,    72,    59,    -1,    71,
      60,    70,    -1,    71,    -1,   104,    -1,    73,    60,    72,
      -1,    73,    -1,   103,    -1,    16,    20,   104,    91,    -1,
      27,   104,    21,    97,    91,    -1,     3,    23,   104,    58,
      99,    59,    88,    -1,     7,    23,   104,    -1,     3,    29,
     104,    -1,     7,    29,   104,    -1,    30,    29,   104,    -1,
      31,    29,   104,    -1,     4,   104,    58,   104,    59,    -1,
       6,   104,    58,   104,    59,    -1,     6,   104,    -1,     9,
      23,   104,    20,    58,    55,    59,    -1,     8,    23,   104,
      -1,    10,    90,    -1,    11,    57,    -1,    17,   104,    18,
      41,    52,    -1,   105,    -1,    13,   104,    -1,   105,    -1,
      23,   104,    -1,   105,    -1,    14,    92,    -1,   105,    -1,
      93,    -1,    94,    -1,    96,   102,   103,    -1,    96,   102,
      96,    -1,    58,    95,    59,    -1,    96,    60,    95,    -1,
      96,    -1,   104,    61,   104,    -1,   104,    61,    62,    -1,
     104,    -1,    62,    -1,    98,    60,    97,    -1,    98,    -1,
     104,    41,   103,    -1,   100,    60,    99,    -1,   100,    -1,
     104,    34,    58,   103,    59,   101,    -1,   104,    35,    58,
     103,    59,   101,    -1,   104,    32,   101,    -1,   104,    33,
     101,    -1,   104,    36,   101,    -1,   104,    37,   101,    -1,
     104,    38,   101,    -1,   104,    39,   101,    -1,   104,    40,
     101,    -1,   105,    -1,    26,    50,    -1,    42,    -1,    43,
      -1,    44,    -1,    45,    -1,    41,    -1,    46,    -1,    48,
      -1,    49,    -1,    52,    -1,    53,    -1,    55,    -1,    54,
      -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   141,   141,   146,   154,   160,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   181,   182,   183,
     184,   185,   186,   193,   214,   218,   222,   229,   233,   240,
     249,   253,   260,   267,   271,   278,   285,   292,   300,   307,
     314,   321,   328,   335,   342,   358,   362,   369,   375,   382,
     389,   396,   400,   407,   411,   418,   422,   429,   433,   440,
     441,   445,   452,   459,   463,   467,   474,   478,   482,   486,
     493,   497,   504,   511,   515,   522,   526,   530,   534,   539,
     543,   547,   551,   555,   562,   566,   573,   577,   581,   585,
     589,   593,   597,   601,   608,   612,   616,   623,   629
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "RW_CREATE", "RW_BUILD", "RW_REBUILD",
  "RW_DROP", "RW_DESTROY", "RW_PRINT", "RW_LOAD", "RW_HELP", "RW_QUIT",
  "RW_SELECT", "RW_INTO", "RW_WHERE", "RW_INSERT", "RW_DELETE",
  "RW_PRIMARY", "RW_NUMBUCKETS", "RW_ALL", "RW_FROM", "RW_SET", "RW_AS",
  "RW_TABLE", "RW_AND", "RW_OR", "RW_NOT", "RW_UPDATE", "RW_VALUES",
  "RW_DATABASE", "RW_OPEN", "RW_CLOSE", "INT_TYPE", "REAL_TYPE",
  "CHAR_TYPE", "VCHAR_TYPE", "DATE_TYPE", "DATETIME_TYPE", "BOOLEAN_TYPE",
  "BLOB_TYPE", "CLOB_TYPE", "T_EQ", "T_LT", "T_LE", "T_GT", "T_GE", "T_NE",
  "T_EOF", "T_ISNULL", "T_ISNOTNULL", "T_NULL", "NOTOKEN", "T_INT",
  "T_REAL", "T_STRING", "T_QSTRING", "T_SHELL_CMD", "';'", "'('", "')'",
  "','", "'.'", "'*'", "$accept", "start", "command", "query",
  "table_list", "table", "insert", "attrib_list", "attrib", "value_list",
  "val", "delete", "update", "createrel", "destroyrel", "createdb",
  "destroydb", "opendb", "closedb", "build", "drop", "load", "print",
  "help", "quit", "opt_primary_attr", "opt_into_relname", "opt_relname",
  "opt_where", "qual", "selection", "join", "non_mt_qualattr_list",
  "qualattr", "non_mt_attrval_list", "attrval", "non_mt_attrtype_list",
  "attrtype", "attrcanbenull", "op", "value", "string", "nothing", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,    59,    40,    41,
      44,    46,    42
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    63,    64,    64,    64,    64,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    66,    67,    67,    67,    68,    68,    69,
      70,    70,    71,    72,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    83,    84,    85,    86,
      87,    88,    88,    89,    89,    90,    90,    91,    91,    92,
      92,    93,    94,    95,    95,    95,    96,    96,    96,    96,
      97,    97,    98,    99,    99,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   101,   101,   102,   102,   102,   102,
     102,   102,   102,   102,   103,   103,   103,   104,   105
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     3,     3,     1,     2,     1,    10,
       3,     1,     1,     3,     1,     1,     4,     5,     7,     3,
       3,     3,     3,     3,     5,     5,     2,     7,     3,     2,
       2,     5,     1,     2,     1,     2,     1,     2,     1,     1,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     1,
       3,     1,     3,     3,     1,     6,     6,     3,     3,     3,
       3,     3,     3,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     4,     0,     0,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     5,     3,     0,     0,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,     0,     0,    97,
       0,    46,     0,     0,     0,     0,     0,    49,    56,    50,
       0,    69,    98,    65,    68,     0,     0,     0,     0,     0,
       1,     2,     0,    40,     0,     0,    39,    41,    48,     0,
      55,     0,     0,     0,    54,     0,     0,     0,    98,     0,
      42,    43,     0,     0,     0,     0,    63,    53,     0,    64,
      67,    66,     0,     0,    36,    58,    98,    71,     0,     0,
      74,     0,    44,    45,     0,     0,    98,    26,    28,     0,
      31,    32,    57,    59,    60,     0,    37,     0,     0,    98,
       0,    98,    98,     0,     0,    98,    98,    98,    98,    98,
       0,     0,    23,     0,    27,     0,     0,    90,    86,    87,
      88,    89,    91,    92,    93,     0,    70,    94,    95,    96,
      72,     0,    38,    52,    73,     0,    77,    84,    78,     0,
       0,    79,    80,    81,    82,    83,    47,    24,    25,     0,
      30,    62,    61,     0,    85,     0,     0,     0,     0,    98,
      98,     0,    34,    35,     0,    75,    76,    29,     0,    51,
      33
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,    18,    19,    20,   106,   107,    21,   109,   110,   181,
     182,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,   152,    73,    47,    94,   112,
     113,   114,    52,    53,    96,    97,    99,   100,   156,   145,
     183,    54,   157
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -116
static const short int yypact[] =
{
     128,  -116,    35,   -45,   -45,    36,     4,     6,    14,   -19,
     -47,    34,    40,   -45,    37,    38,  -116,  -116,    63,    11,
    -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,
    -116,  -116,  -116,  -116,  -116,  -116,  -116,   -45,   -45,  -116,
      12,    16,   -45,   -45,   -45,   -45,   -45,  -116,  -116,  -116,
     -47,  -116,    62,    17,    19,   -45,   -45,    57,   -45,   -45,
    -116,  -116,    23,  -116,   -45,   -45,  -116,  -116,  -116,    64,
    -116,    27,   -45,    67,  -116,   -47,   -46,    24,    74,   -45,
    -116,  -116,   -45,    41,    44,    46,  -116,  -116,    -8,  -116,
    -116,  -116,   -45,   -26,  -116,  -116,    74,    39,    65,    48,
      49,    58,  -116,  -116,    53,    -8,    74,    50,   -45,    54,
      52,  -116,  -116,  -116,  -116,    77,  -116,   -45,    -4,    98,
     -45,    90,    90,    69,    70,    90,    90,    90,    90,    90,
      82,    83,  -116,    -8,  -116,   117,   -45,  -116,  -116,  -116,
    -116,  -116,  -116,  -116,  -116,   -29,  -116,  -116,  -116,  -116,
    -116,   -45,  -116,  -116,  -116,    96,  -116,  -116,  -116,    -4,
      -4,  -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,    89,
    -116,  -116,  -116,   131,  -116,    91,    92,    -4,   111,    90,
      90,    94,    97,  -116,   102,  -116,  -116,  -116,    -4,  -116,
    -116
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
    -116,  -116,  -116,  -116,  -101,  -116,  -116,    20,  -116,   -28,
    -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,
    -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,   -84,  -116,
    -116,  -116,   -44,   -91,    45,  -116,    43,  -116,  -108,  -116,
    -115,    -3,     5
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -99
static const short int yytable[] =
{
      40,    41,   115,   150,   131,    36,    71,    39,    39,    39,
      57,    50,   116,    48,   158,    51,    90,   161,   162,   163,
     164,   165,   132,   147,   148,    39,   149,    44,    39,    45,
     172,    89,   168,    51,    62,    63,    51,    46,    49,    66,
      67,    68,    69,    70,   175,   176,    39,    55,   147,   148,
     105,   149,    77,    78,   171,    80,    81,    74,    37,    42,
      56,    83,    84,    60,    38,    43,    58,    59,    61,    87,
      64,   185,   186,    91,    65,    72,    98,    75,    79,   101,
      76,    82,    92,    95,    85,   108,    86,    88,    93,   111,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   117,
     102,    95,   108,   103,   104,   134,   118,   119,   130,   120,
     133,    95,   136,   135,    98,   151,   155,   101,   137,   138,
     139,   140,   141,   142,   153,   143,   144,   159,   160,     1,
     108,     2,     3,   111,     4,     5,     6,     7,     8,     9,
      10,   166,   167,    11,    12,   169,   174,   177,   173,   178,
     179,   180,   184,   187,   189,    13,   170,   188,    14,    15,
     190,     0,   146,   154,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    16,     0,     0,     0,     0,
       0,     0,     0,     0,    17,   -98
};

static const short int yycheck[] =
{
       3,     4,    93,   118,   105,     0,    50,    54,    54,    54,
      13,    58,    96,     8,   122,    62,    62,   125,   126,   127,
     128,   129,   106,    52,    53,    54,    55,    23,    54,    23,
     145,    75,   133,    62,    37,    38,    62,    23,    57,    42,
      43,    44,    45,    46,   159,   160,    54,    13,    52,    53,
      58,    55,    55,    56,   145,    58,    59,    52,    23,    23,
      20,    64,    65,     0,    29,    29,    29,    29,    57,    72,
      58,   179,   180,    76,    58,    13,    79,    60,    21,    82,
      61,    58,    58,    78,    20,    88,    59,    20,    14,    92,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    60,
      59,    96,   105,    59,    58,   108,    41,    59,    55,    60,
      60,   106,    60,    59,   117,    17,    26,   120,    41,    42,
      43,    44,    45,    46,   119,    48,    49,    58,    58,     1,
     133,     3,     4,   136,     6,     7,     8,     9,    10,    11,
      12,    59,    59,    15,    16,    28,    50,    58,   151,    18,
      59,    59,    41,    59,    52,    27,   136,    60,    30,    31,
     188,    -1,   117,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    57
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     1,     3,     4,     6,     7,     8,     9,    10,    11,
      12,    15,    16,    27,    30,    31,    47,    56,    64,    65,
      66,    69,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,   105,    23,    29,    54,
     104,   104,    23,    29,    23,    23,    23,    90,   105,    57,
      58,    62,    95,    96,   104,    13,    20,   104,    29,    29,
       0,    57,   104,   104,    58,    58,   104,   104,   104,   104,
     104,    95,    13,    89,   105,    60,    61,   104,   104,    21,
     104,   104,    58,   104,   104,    20,    59,   104,    20,    95,
      62,   104,    58,    14,    91,   105,    97,    98,   104,    99,
     100,   104,    59,    59,    58,    58,    67,    68,   104,    70,
      71,   104,    92,    93,    94,    96,    91,    60,    41,    59,
      60,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      55,    67,    91,    60,   104,    59,    60,    41,    42,    43,
      44,    45,    46,    48,    49,   102,    97,    52,    53,    55,
     103,    17,    88,   105,    99,    26,   101,   105,   101,    58,
      58,   101,   101,   101,   101,   101,    59,    59,    67,    28,
      70,    96,   103,   104,    50,   103,   103,    58,    18,    59,
      59,    72,    73,   103,    41,   101,   101,    59,    60,    52,
      72
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
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
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
    while (0)
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
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

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
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
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
      size_t yyn = 0;
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

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
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
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

/*  switch (yytype)
    {

      default:
        break;
    }*/
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()
    ;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


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
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
        case 2:
#line 142 "parse.ypp"
    {
		parse_tree = (yyvsp[-1].n);
		YYACCEPT;
	;}
    break;

  case 3:
#line 147 "parse.ypp"
    {
	        if(!isatty(0))
		    puts((yyvsp[0].sval));
		(void)system((yyvsp[0].sval));
		parse_tree = NULL;
		YYACCEPT;
	;}
    break;

  case 4:
#line 155 "parse.ypp"
    {
		reset_scanner();
		parse_tree = NULL;
		YYACCEPT;
	;}
    break;

  case 5:
#line 161 "parse.ypp"
    {
		quit();
	;}
    break;

  case 22:
#line 187 "parse.ypp"
    {
		(yyval.n) = NULL;
	;}
    break;

  case 23:
#line 195 "parse.ypp"
    {
		NODE *where;
		NODE *qualattr_list = replace_alias_in_qualattr_list((yyvsp[-1].n), (yyvsp[-4].n));
		if (qualattr_list == NULL) { // something wrong in qualattr_list
		  (yyval.n) = NULL;
		}
		else {
		  where = replace_alias_in_condition((yyvsp[-1].n), (yyvsp[0].n));
		  if ((where == NULL) && ((yyvsp[0].n) != NULL)) {
		     (yyval.n) = NULL; //something wrong in where condition
		  }
		  else {
		    (yyval.n) = query_node((yyvsp[-3].sval), qualattr_list, where);
		  }
		}
	;}
    break;

  case 24:
#line 215 "parse.ypp"
    {
		(yyval.n) = (yyvsp[-1].n);
	;}
    break;

  case 25:
#line 219 "parse.ypp"
    {
		(yyval.n) = prepend((yyvsp[-2].n), (yyvsp[0].n));
	;}
    break;

  case 26:
#line 223 "parse.ypp"
    {
		(yyval.n) = list_node((yyvsp[0].n));
	;}
    break;

  case 27:
#line 230 "parse.ypp"
    {
		(yyval.n) = alias_node((yyvsp[-1].sval), (yyvsp[0].sval));
	;}
    break;

  case 28:
#line 234 "parse.ypp"
    {
		(yyval.n) = alias_node((yyvsp[0].sval), NULL);
	;}
    break;

  case 29:
#line 241 "parse.ypp"
    {
		NODE* tmp = merge_attr_value_list((yyvsp[-5].n), (yyvsp[-1].n));
		if (tmp == NULL) (yyval.n)=NULL;
		else (yyval.n) = insert_node((yyvsp[-7].sval), tmp);
	;}
    break;

  case 30:
#line 250 "parse.ypp"
    {
		(yyval.n) = prepend((yyvsp[-2].n), (yyvsp[0].n));
	;}
    break;

  case 31:
#line 254 "parse.ypp"
    {
		(yyval.n) = list_node((yyvsp[0].n));
	;}
    break;

  case 32:
#line 261 "parse.ypp"
    {
		(yyval.n) = attrval_node((yyvsp[0].sval), NULL);
	;}
    break;

  case 33:
#line 268 "parse.ypp"
    {
		(yyval.n) = prepend((yyvsp[-2].n), (yyvsp[0].n));
	;}
    break;

  case 34:
#line 272 "parse.ypp"
    {
		(yyval.n) = list_node((yyvsp[0].n));
	;}
    break;

  case 35:
#line 279 "parse.ypp"
    {
		(yyval.n) = (yyvsp[0].n);
	;}
    break;

  case 36:
#line 286 "parse.ypp"
    { 
		(yyval.n) = delete_node((yyvsp[-1].sval), (yyvsp[0].n));
	;}
    break;

  case 37:
#line 293 "parse.ypp"
    {
		(yyval.n) = update_node((yyvsp[-3].sval),(yyvsp[-1].n),(yyvsp[0].n));
	;}
    break;

  case 38:
#line 301 "parse.ypp"
    {
		(yyval.n) = createRel_node((yyvsp[-4].sval), (yyvsp[-2].n), (yyvsp[0].n));
	;}
    break;

  case 39:
#line 308 "parse.ypp"
    {
		(yyval.n) = destroyRel_node((yyvsp[0].sval));
	;}
    break;

  case 40:
#line 315 "parse.ypp"
    {
		(yyval.n) = createDB_node((yyvsp[0].sval));
	;}
    break;

  case 41:
#line 322 "parse.ypp"
    {
		(yyval.n) = destroyDB_node((yyvsp[0].sval));
	;}
    break;

  case 42:
#line 329 "parse.ypp"
    {
		(yyval.n) = openDB_node((yyvsp[0].sval));
	;}
    break;

  case 43:
#line 336 "parse.ypp"
    {
		(yyval.n) = closeDB_node((yyvsp[0].sval));
	;}
    break;

  case 44:
#line 343 "parse.ypp"
    {
		(yyval.n) = build_node((yyvsp[-3].sval), (yyvsp[-1].sval), 0);
	;}
    break;

  case 45:
#line 359 "parse.ypp"
    {
		(yyval.n) = drop_node((yyvsp[-3].sval), (yyvsp[-1].sval));
	;}
    break;

  case 46:
#line 363 "parse.ypp"
    {
		(yyval.n) = drop_node((yyvsp[0].sval), NULL);
	;}
    break;

  case 47:
#line 370 "parse.ypp"
    {
		(yyval.n) = load_node((yyvsp[-4].sval), (yyvsp[-1].sval));
	;}
    break;

  case 48:
#line 376 "parse.ypp"
    {
		(yyval.n) = print_node((yyvsp[0].sval));
	;}
    break;

  case 49:
#line 383 "parse.ypp"
    {
		(yyval.n) = help_node((yyvsp[0].sval));
	;}
    break;

  case 50:
#line 390 "parse.ypp"
    {
		quit();
	;}
    break;

  case 51:
#line 397 "parse.ypp"
    {
		(yyval.n) = primattr_node((yyvsp[-3].sval), (yyvsp[0].ival));
	;}
    break;

  case 52:
#line 401 "parse.ypp"
    {
		(yyval.n) = NULL;
	;}
    break;

  case 53:
#line 408 "parse.ypp"
    {
		(yyval.sval) = (yyvsp[0].sval);
	;}
    break;

  case 54:
#line 412 "parse.ypp"
    {
		(yyval.sval) = NULL;
	;}
    break;

  case 55:
#line 419 "parse.ypp"
    {
		(yyval.sval) = (yyvsp[0].sval);
	;}
    break;

  case 56:
#line 423 "parse.ypp"
    {
		(yyval.sval) = NULL;
	;}
    break;

  case 57:
#line 430 "parse.ypp"
    {
		(yyval.n) = (yyvsp[0].n);
	;}
    break;

  case 58:
#line 434 "parse.ypp"
    {
		(yyval.n) = NULL;
	;}
    break;

  case 61:
#line 446 "parse.ypp"
    {
		(yyval.n) = select_node((yyvsp[-2].n), (yyvsp[-1].ival), (yyvsp[0].n));
	;}
    break;

  case 62:
#line 453 "parse.ypp"
    {
		(yyval.n) = join_node((yyvsp[-2].n), (yyvsp[-1].ival), (yyvsp[0].n));
	;}
    break;

  case 63:
#line 460 "parse.ypp"
    {
		(yyval.n) = (yyvsp[-1].n);
	;}
    break;

  case 64:
#line 464 "parse.ypp"
    {
		(yyval.n) = prepend((yyvsp[-2].n), (yyvsp[0].n));
	;}
    break;

  case 65:
#line 468 "parse.ypp"
    {
		(yyval.n) = list_node((yyvsp[0].n));
	;}
    break;

  case 66:
#line 475 "parse.ypp"
    {
		(yyval.n) = qualattr_node((yyvsp[-2].sval), (yyvsp[0].sval));
	;}
    break;

  case 67:
#line 479 "parse.ypp"
    {
		(yyval.n) = qualattr_node((yyvsp[-2].sval), "*");
	;}
    break;

  case 68:
#line 483 "parse.ypp"
    {
		(yyval.n) = qualattr_node(NULL, (yyvsp[0].sval));
	;}
    break;

  case 69:
#line 487 "parse.ypp"
    {
                (yyval.n) = qualattr_node(NULL, "*");
        ;}
    break;

  case 70:
#line 494 "parse.ypp"
    {
		(yyval.n) = prepend((yyvsp[-2].n), (yyvsp[0].n));
	;}
    break;

  case 71:
#line 498 "parse.ypp"
    {
		(yyval.n) = list_node((yyvsp[0].n));
	;}
    break;

  case 72:
#line 505 "parse.ypp"
    {
		(yyval.n) = attrval_node((yyvsp[-2].sval), (yyvsp[0].n));
	;}
    break;

  case 73:
#line 512 "parse.ypp"
    {
		(yyval.n) = prepend((yyvsp[-2].n), (yyvsp[0].n));
	;}
    break;

  case 74:
#line 516 "parse.ypp"
    {
		(yyval.n) = list_node((yyvsp[0].n));
	;}
    break;

  case 75:
#line 523 "parse.ypp"
    {
	 	(yyval.n) = attrtype_node((yyvsp[-5].sval),0,(yyvsp[-2].n)->u.VALUE.u.ival,(yyvsp[0].n)->u.VALUE.u.ival);
	;}
    break;

  case 76:
#line 527 "parse.ypp"
    {
		(yyval.n) = attrtype_node((yyvsp[-5].sval),1, (yyvsp[-2].n)->u.VALUE.u.ival,(yyvsp[0].n)->u.VALUE.u.ival);
	;}
    break;

  case 77:
#line 531 "parse.ypp"
    {
		(yyval.n) = attrtype_node((yyvsp[-2].sval),2,SIZEOFINT,(yyvsp[0].n)->u.VALUE.u.ival);
	;}
    break;

  case 78:
#line 535 "parse.ypp"
    {
		(yyval.n) = attrtype_node((yyvsp[-2].sval),3,SIZEOFFLOAT,(yyvsp[0].n)->u.VALUE.u.ival);
	;}
    break;

  case 79:
#line 540 "parse.ypp"
    {
		(yyval.n) = attrtype_node((yyvsp[-2].sval), 4,SIZEOFDATE,(yyvsp[0].n)->u.VALUE.u.ival);
	;}
    break;

  case 80:
#line 544 "parse.ypp"
    {
		(yyval.n) = attrtype_node((yyvsp[-2].sval), 5,SIZEOFDATETIME,(yyvsp[0].n)->u.VALUE.u.ival);
	;}
    break;

  case 81:
#line 548 "parse.ypp"
    {
		(yyval.n) = attrtype_node((yyvsp[-2].sval), 6,SIZEOFBOOL,(yyvsp[0].n)->u.VALUE.u.ival);
	;}
    break;

  case 82:
#line 552 "parse.ypp"
    {
		(yyval.n) = attrtype_node((yyvsp[-2].sval), 7,0,(yyvsp[0].n)->u.VALUE.u.ival);
	;}
    break;

  case 83:
#line 556 "parse.ypp"
    {
		(yyval.n) = attrtype_node((yyvsp[-2].sval), 8,0,(yyvsp[0].n)->u.VALUE.u.ival);
	;}
    break;

  case 84:
#line 563 "parse.ypp"
    {
		(yyval.n) = int_node(0);
	;}
    break;

  case 85:
#line 567 "parse.ypp"
    {
		(yyval.n) = int_node(1);
	;}
    break;

  case 86:
#line 574 "parse.ypp"
    {
		(yyval.ival) = LT;
	;}
    break;

  case 87:
#line 578 "parse.ypp"
    {
		(yyval.ival) = LTE;
	;}
    break;

  case 88:
#line 582 "parse.ypp"
    {
		(yyval.ival) = GT;
	;}
    break;

  case 89:
#line 586 "parse.ypp"
    {
		(yyval.ival) = GTE;
	;}
    break;

  case 90:
#line 590 "parse.ypp"
    {
		(yyval.ival) = EQ;
	;}
    break;

  case 91:
#line 594 "parse.ypp"
    {
		(yyval.ival) = NE;
	;}
    break;

  case 92:
#line 598 "parse.ypp"
    {
		(yyval.ival) = ISNULL;
	;}
    break;

  case 93:
#line 602 "parse.ypp"
    {
		(yyval.ival) = ISNOTNULL;
	;}
    break;

  case 94:
#line 609 "parse.ypp"
    {
		(yyval.n) = int_node((yyvsp[0].ival));
	;}
    break;

  case 95:
#line 613 "parse.ypp"
    {
		(yyval.n) = float_node((yyvsp[0].rval));
	;}
    break;

  case 96:
#line 617 "parse.ypp"
    {
		(yyval.n) = string_node((yyvsp[0].sval));
	;}
    break;

  case 97:
#line 624 "parse.ypp"
    {
		(yyval.sval) = (yyvsp[0].sval);
	;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 1976 "parse.tab.cpp"

  yyvsp -= yylen;
  yyssp -= yylen;


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
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
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
	  int yychecklim = YYLAST - yyn;
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
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
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
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
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


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 633 "parse.ypp"


void parse(void)
{
  extern void new_query();
  extern void interp(NODE *);

  for(;;){
    yylineno=1;
    // reset parser and scanner for a new query
    new_query();

    // print a prompt
    printf("%s", PROMPT);
    fflush(stdout);

    // if a query was successfully read, interpret it
    if(yyparse() == 0 && parse_tree != NULL)
      interp(parse_tree);
  }
}

void yyerror(char *s)
{
    printf("SQL Command Error: %s encountered at line:%d\n", s, yylineno);
    //puts(s);
}

