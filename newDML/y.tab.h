#ifndef BISON_Y_TAB_H
# define BISON_Y_TAB_H
#include "parse.h"
#ifndef YYSTYPE
typedef union{
  int ival;
  float rval;
  char *sval;
  NODE *n;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif

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
#define INT_TYPE 284
#define REAL_TYPE 285
#define CHAR_TYPE 286
#define T_EQ 287
#define T_LT 288
#define T_LE 289
#define T_GT 290
#define T_GE 291
#define T_NE 292
#define T_EOF 293
#define NOTOKEN 294
#define T_INT 295
#define T_REAL 296
#define T_STRING 297
#define T_QSTRING 298
#define T_SHELL_CMD 299


extern YYSTYPE yylval;

#endif /* not BISON_Y_TAB_H */
