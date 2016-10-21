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
     T_TRUE = 306,
     T_FALSE = 307,
     NOTOKEN = 308,
     T_INT = 309,
     T_REAL = 310,
     T_STRING = 311,
     T_QSTRING = 312,
     T_SHELL_CMD = 313
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
#define T_TRUE 306
#define T_FALSE 307
#define NOTOKEN 308
#define T_INT 309
#define T_REAL 310
#define T_STRING 311
#define T_QSTRING 312
#define T_SHELL_CMD 313




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 24 "parse.ypp"
typedef union YYSTYPE {
  int ival;
  float rval;
  char *sval;
  NODE *n;
} YYSTYPE;
/* Line 1447 of yacc.c.  */
#line 161 "parse.tab.hpp"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



