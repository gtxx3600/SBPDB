
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
#line 1 "gram.y"


#include <stdio.h>

#include "sbpdb.h"
#include "planner.h"
#include "ql.h"
#include "sm.h"

#include "lex.h"

Expression *translateQuery(RelAttrList *al, IDList *rl, Condition *cond) {
	Expression *rels = NULL;
	IDList *p = rl;
	Expression *selexp = NEW(Expression), *ret;
	selexp = NEW(Expression);
	selexp->kind = SelectionExp;
	selexp->u.sele = NEW(struct selection_exp);
	selexp->u.sele->cond = cond;
	while (p) {
		if (rels) {
			Expression *tmp = NEW(Expression);
			Expression *left, *right;
			tmp->kind = ProductExp;
			tmp->u.prode = NEW(struct product_exp);
			left = rels;
			right = NEW(Expression);
			right->kind = Relation;
			right->u.rel = NEW(struct relation);
			right->u.rel->id = p->id;
			tmp->u.prode->left = left;
			tmp->u.prode->right = right;
			rels = tmp;
		} else {
			rels = NEW(Expression);
			rels->kind = Relation;
			rels->u.rel = NEW(struct relation);
			rels->u.rel->id = p->id;
		}
		p = p->next;
	}
	selexp->u.sele->exp = rels;
	if (al) {
		ret = NEW(Expression);
		ret->kind = ProjectionExp;
		ret->u.proje = NEW(struct projection_exp);
		ret->u.proje->al = al;
		ret->u.proje->exp = selexp;
		return ret;
	} else {
		return selexp;
	}
}

void yyerror(const char *str) {
}

void prterror(const char *str) {
    fprintf(stderr, "Error: %s\n", str);
}

PF_Manager *pfManager;
SM_Manager *smManager;
QL_Manager *qlManager;



/* Line 189 of yacc.c  */
#line 141 "gram.c"

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
     SEMICOLON = 258,
     CREATE = 259,
     DROP = 260,
     TABLE = 261,
     VIEW = 262,
     SELECT = 263,
     INSERT = 264,
     DELETE = 265,
     UPDATE = 266,
     LBRACE = 267,
     RBRACE = 268,
     COMMA = 269,
     INT_T = 270,
     STRING_T = 271,
     FLOAT_T = 272,
     HELP = 273,
     EXIT = 274,
     DOT = 275,
     INTO = 276,
     VALUES = 277,
     STAR = 278,
     FROM = 279,
     WHERE = 280,
     AND = 281,
     OR = 282,
     NOT = 283,
     AS = 284,
     PRIMARY = 285,
     KEY = 286,
     EQ = 287,
     LT = 288,
     GT = 289,
     LE = 290,
     GE = 291,
     NE = 292,
     IN = 293,
     DATABASE = 294,
     USE = 295,
     SET = 296,
     FOREIGN = 297,
     REFERENCES = 298,
     CHECK = 299,
     SHOW = 300,
     DATABASES = 301,
     TABLES = 302,
     ROLLBACK = 303,
     COMMIT = 304,
     NUMBER = 305,
     ID = 306,
     STRING_V = 307,
     QUERY = 308
   };
#endif
/* Tokens.  */
#define SEMICOLON 258
#define CREATE 259
#define DROP 260
#define TABLE 261
#define VIEW 262
#define SELECT 263
#define INSERT 264
#define DELETE 265
#define UPDATE 266
#define LBRACE 267
#define RBRACE 268
#define COMMA 269
#define INT_T 270
#define STRING_T 271
#define FLOAT_T 272
#define HELP 273
#define EXIT 274
#define DOT 275
#define INTO 276
#define VALUES 277
#define STAR 278
#define FROM 279
#define WHERE 280
#define AND 281
#define OR 282
#define NOT 283
#define AS 284
#define PRIMARY 285
#define KEY 286
#define EQ 287
#define LT 288
#define GT 289
#define LE 290
#define GE 291
#define NE 292
#define IN 293
#define DATABASE 294
#define USE 295
#define SET 296
#define FOREIGN 297
#define REFERENCES 298
#define CHECK 299
#define SHOW 300
#define DATABASES 301
#define TABLES 302
#define ROLLBACK 303
#define COMMIT 304
#define NUMBER 305
#define ID 306
#define STRING_V 307
#define QUERY 308




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 74 "gram.y"

    AttrInfo *attrInfo;
    AttrType attrType;
    Value *value;
	ValueList *valueList;
    RelAttr *relAttr;
	RelAttrList *relAttrList;
	RelAttrValue *relAttrValue;
	RelAttrValueList *relAttrValueList;
	AssignmentList *assignmentList;
	IDList *idList;
    Condition *condition;
	CheckCondition *checkCondition;
	Expression *expression;
    int compOp;
    int number;
    char *string;



/* Line 214 of yacc.c  */
#line 304 "gram.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 316 "gram.c"

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
#define YYFINAL  58
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   168

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  86
/* YYNRULES -- Number of states.  */
#define YYNSTATES  178

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   308

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    13,    15,    17,    19,
      21,    23,    25,    27,    29,    31,    33,    35,    37,    39,
      41,    43,    45,    47,    50,    53,    57,    61,    64,    67,
      71,    76,    81,    89,    94,   101,   106,   108,   112,   118,
     119,   122,   123,   131,   132,   139,   141,   143,   145,   154,
     156,   160,   162,   164,   168,   175,   180,   186,   194,   198,
     204,   211,   216,   219,   221,   223,   227,   233,   237,   241,
     244,   248,   250,   252,   254,   256,   258,   260,   262,   264,
     266,   270,   272,   276,   278,   282,   284
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      55,     0,    -1,    56,    -1,    55,    56,    -1,     1,     3,
      -1,    57,    -1,    60,    -1,    58,    -1,    59,    -1,    63,
      -1,    64,    -1,    66,    -1,    68,    -1,    65,    -1,    67,
      -1,    69,    -1,    83,    -1,    76,    -1,    79,    -1,    80,
      -1,    62,    -1,    61,    -1,     3,    -1,    49,     3,    -1,
      48,     3,    -1,    45,    46,     3,    -1,    45,    47,     3,
      -1,    19,     3,    -1,    18,     3,    -1,    40,    51,     3,
      -1,     4,    39,    51,     3,    -1,     5,    39,    51,     3,
      -1,     4,     6,    51,    12,    70,    13,     3,    -1,     5,
       6,    51,     3,    -1,     4,     7,    51,    29,    82,     3,
      -1,     5,     7,    51,     3,    -1,    71,    -1,    70,    14,
      71,    -1,    51,    75,    72,    73,    74,    -1,    -1,    30,
      31,    -1,    -1,    42,    31,    43,    51,    12,    51,    13,
      -1,    -1,    44,    12,    51,    86,    78,    13,    -1,    15,
      -1,    16,    -1,    17,    -1,     9,    21,    51,    22,    12,
      77,    13,     3,    -1,    78,    -1,    77,    14,    78,    -1,
      50,    -1,    52,    -1,    50,    20,    50,    -1,    10,    24,
      51,    25,    85,     3,    -1,    10,    24,    51,     3,    -1,
      10,    23,    24,    51,     3,    -1,    11,    51,    41,    81,
      25,    85,     3,    -1,    89,    32,    87,    -1,    81,    14,
      89,    32,    87,    -1,     8,    84,    24,    91,    25,    85,
      -1,     8,    84,    24,    91,    -1,    82,     3,    -1,    23,
      -1,    90,    -1,    87,    86,    87,    -1,    88,    38,    12,
      82,    13,    -1,    85,    26,    85,    -1,    85,    27,    85,
      -1,    28,    85,    -1,    12,    85,    13,    -1,    32,    -1,
      33,    -1,    34,    -1,    35,    -1,    36,    -1,    37,    -1,
      89,    -1,    78,    -1,    87,    -1,    88,    14,    87,    -1,
      51,    -1,    51,    20,    51,    -1,    89,    -1,    90,    14,
      89,    -1,    51,    -1,    91,    14,    51,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   123,   123,   125,   130,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   153,   157,   163,   169,   172,   180,   187,   193,
     202,   211,   221,   237,   249,   263,   275,   278,   285,   300,
     301,   307,   308,   316,   317,   326,   329,   332,   337,   355,
     360,   370,   377,   385,   391,   401,   411,   424,   441,   447,
     457,   460,   466,   474,   477,   483,   491,   498,   505,   512,
     518,   524,   525,   526,   527,   528,   529,   533,   538,   545,
     550,   560,   565,   572,   577,   586,   591
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEMICOLON", "CREATE", "DROP", "TABLE",
  "VIEW", "SELECT", "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE",
  "COMMA", "INT_T", "STRING_T", "FLOAT_T", "HELP", "EXIT", "DOT", "INTO",
  "VALUES", "STAR", "FROM", "WHERE", "AND", "OR", "NOT", "AS", "PRIMARY",
  "KEY", "EQ", "LT", "GT", "LE", "GE", "NE", "IN", "DATABASE", "USE",
  "SET", "FOREIGN", "REFERENCES", "CHECK", "SHOW", "DATABASES", "TABLES",
  "ROLLBACK", "COMMIT", "NUMBER", "ID", "STRING_V", "QUERY", "$accept",
  "commands", "command", "empty", "commit", "rollback", "show", "exit",
  "help", "use_database", "create_database", "drop_database",
  "create_table", "drop_table", "create_view", "drop_view",
  "attr_def_list", "attr_def", "pk", "fk", "ck", "type", "insert",
  "value_list", "value", "delete", "update", "assignment_list", "query",
  "select", "select_attr", "condition", "compOp", "av", "avlist", "attr",
  "attr_list", "rel_list", 0
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    54,    55,    55,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    57,    58,    59,    60,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    70,    71,    72,
      72,    73,    73,    74,    74,    75,    75,    75,    76,    77,
      77,    78,    78,    78,    79,    79,    79,    80,    81,    81,
      82,    82,    83,    84,    84,    85,    85,    85,    85,    85,
      85,    86,    86,    86,    86,    86,    86,    87,    87,    88,
      88,    89,    89,    90,    90,    91,    91
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     3,     2,     2,     3,
       4,     4,     7,     4,     6,     4,     1,     3,     5,     0,
       2,     0,     7,     0,     6,     1,     1,     1,     8,     1,
       3,     1,     1,     3,     6,     4,     5,     7,     3,     5,
       6,     4,     2,     1,     1,     3,     5,     3,     3,     2,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     3,     1,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     2,     5,     7,     8,
       6,    21,    20,     9,    10,    13,    11,    14,    12,    15,
      17,    18,    19,     0,    16,     4,     0,     0,     0,     0,
       0,     0,    63,    81,     0,    83,    64,     0,     0,     0,
       0,    28,    27,     0,     0,     0,    24,    23,     1,     3,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    25,    26,     0,     0,    30,
      33,    35,    31,    82,    85,    61,    84,     0,     0,    55,
       0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
      56,     0,     0,    51,    52,    78,     0,    79,     0,    77,
       0,     0,     0,    45,    46,    47,    39,     0,     0,    34,
      86,    60,     0,    49,     0,    69,     0,    54,     0,     0,
      71,    72,    73,    74,    75,    76,     0,     0,     0,     0,
       0,    58,     0,    41,    32,    37,     0,     0,    70,    53,
      67,    68,    65,    80,     0,     0,    57,    40,     0,    43,
      48,    50,     0,    59,     0,     0,    38,    66,     0,     0,
       0,     0,     0,     0,     0,     0,    42,    44
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    94,    95,   143,   159,
     166,   116,    30,   122,   105,    31,    32,    91,    33,    34,
      44,   106,   136,   107,   108,   109,    46,    85
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -103
static const yytype_int16 yypact[] =
{
      22,    16,  -103,    10,    39,   -14,    26,    57,   -27,    60,
      86,    58,    55,    92,   105,     3,  -103,  -103,  -103,  -103,
    -103,  -103,  -103,  -103,  -103,  -103,  -103,  -103,  -103,  -103,
    -103,  -103,  -103,   107,  -103,  -103,    61,    62,    63,    64,
      66,    67,  -103,    91,    95,  -103,   102,    69,    97,    71,
      82,  -103,  -103,   121,   122,   123,  -103,  -103,  -103,  -103,
    -103,   115,    99,   126,   127,   128,   129,    83,    85,    87,
     111,    88,    41,    87,  -103,  -103,  -103,    89,   133,  -103,
    -103,  -103,  -103,  -103,  -103,    40,  -103,   125,   132,  -103,
       8,    65,   110,    76,    84,  -103,   140,    94,     8,    44,
    -103,     8,     8,   124,  -103,  -103,     2,    51,     4,  -103,
      87,     8,     5,  -103,  -103,  -103,   116,   144,    89,  -103,
    -103,    77,    93,  -103,    48,  -103,    98,  -103,     8,     8,
    -103,  -103,  -103,  -103,  -103,  -103,     5,     5,   137,   118,
      12,  -103,   120,   112,  -103,  -103,   149,    44,  -103,  -103,
    -103,   130,  -103,  -103,   133,     5,  -103,  -103,   131,   109,
    -103,  -103,   142,  -103,   114,   146,  -103,  -103,   108,   113,
     148,    51,   117,    44,   150,   152,  -103,  -103
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -103,  -103,   151,  -103,  -103,  -103,  -103,  -103,  -103,  -103,
    -103,  -103,  -103,  -103,  -103,  -103,  -103,    43,  -103,  -103,
    -103,  -103,  -103,  -103,   -97,  -103,  -103,  -103,   -77,  -103,
    -103,   -29,    -4,  -102,  -103,    -5,  -103,  -103
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      45,    96,   123,    58,     1,   127,     2,     3,     4,    42,
     141,     5,     6,     7,     8,   156,    36,    37,   137,    35,
     101,     9,    10,     1,    50,     2,     3,     4,   128,   129,
       5,     6,     7,     8,   152,   153,   102,    43,   128,   129,
       9,    10,   138,    11,    89,    39,    40,    47,    12,    38,
     161,    13,    14,   163,    97,   103,    43,   104,   103,    43,
     104,   148,    11,    51,    86,    98,    90,    12,    92,   121,
      13,    14,   124,   125,   128,   129,   175,   162,    41,   110,
      48,    49,   140,   130,   131,   132,   133,   134,   135,    52,
     111,   113,   114,   115,   103,    56,   104,   117,   118,   150,
     151,    54,    55,   128,   129,   139,   146,   147,    57,    53,
      60,    67,    61,    62,    63,    64,    69,    65,    66,    68,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    87,    83,   100,    84,    99,    43,    88,
      93,     5,   112,   119,   126,   120,   142,   144,   149,   154,
     155,   157,   160,   165,   158,   167,   128,   168,   169,   170,
     172,   145,   164,   176,   171,   177,    59,   173,   174
};

static const yytype_uint8 yycheck[] =
{
       5,    78,    99,     0,     1,     3,     3,     4,     5,    23,
     112,     8,     9,    10,    11,     3,     6,     7,    14,     3,
      12,    18,    19,     1,    51,     3,     4,     5,    26,    27,
       8,     9,    10,    11,   136,   137,    28,    51,    26,    27,
      18,    19,    38,    40,     3,     6,     7,    21,    45,    39,
     147,    48,    49,   155,    14,    50,    51,    52,    50,    51,
      52,    13,    40,     3,    69,    25,    25,    45,    73,    98,
      48,    49,   101,   102,    26,    27,   173,   154,    39,    14,
      23,    24,   111,    32,    33,    34,    35,    36,    37,     3,
      25,    15,    16,    17,    50,     3,    52,    13,    14,   128,
     129,    46,    47,    26,    27,   110,    13,    14,     3,    51,
       3,    20,    51,    51,    51,    51,    14,    51,    51,    24,
      51,    24,    51,    41,     3,     3,     3,    12,    29,     3,
       3,     3,     3,    22,    51,     3,    51,    12,    51,    51,
      51,     8,    32,     3,    20,    51,    30,     3,    50,    12,
      32,    31,     3,    44,    42,    13,    26,    43,    12,    51,
      12,   118,    31,    13,    51,    13,    15,   171,    51
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     8,     9,    10,    11,    18,
      19,    40,    45,    48,    49,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      76,    79,    80,    82,    83,     3,     6,     7,    39,     6,
       7,    39,    23,    51,    84,    89,    90,    21,    23,    24,
      51,     3,     3,    51,    46,    47,     3,     3,     0,    56,
       3,    51,    51,    51,    51,    51,    51,    20,    24,    14,
      51,    24,    51,    41,     3,     3,     3,    12,    29,     3,
       3,     3,     3,    51,    51,    91,    89,    22,    51,     3,
      25,    81,    89,    51,    70,    71,    82,    14,    25,    12,
       3,    12,    28,    50,    52,    78,    85,    87,    88,    89,
      14,    25,    32,    15,    16,    17,    75,    13,    14,     3,
      51,    85,    77,    78,    85,    85,    20,     3,    26,    27,
      32,    33,    34,    35,    36,    37,    86,    14,    38,    89,
      85,    87,    30,    72,     3,    71,    13,    14,    13,    50,
      85,    85,    87,    87,    12,    32,     3,    31,    42,    73,
       3,    78,    82,    87,    31,    44,    74,    13,    43,    12,
      51,    51,    12,    86,    51,    78,    13,    13
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
        case 2:

/* Line 1455 of yacc.c  */
#line 123 "gram.y"
    {
	}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 125 "gram.y"
    {
	}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 130 "gram.y"
    {
		prterror("syntax error");
	}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 157 "gram.y"
    {
		//PF_Commit();
	}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 163 "gram.y"
    {
		//PF_Rollback();
	}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 169 "gram.y"
    {
		SM_ShowDbs(smManager);
	}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 172 "gram.y"
    {
		if (SM_ShowTbls(smManager)) {
			fprintf(stderr, "Error: no database selected\n");
		}
	}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 180 "gram.y"
    {
        SM_Exit(smManager);
        return 0;
    }
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 187 "gram.y"
    {
        SM_Help(smManager);
    }
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 193 "gram.y"
    {
		int ret;
		if ((ret = SM_UseDatabase(smManager, (yyvsp[(2) - (3)].string))) != NORMAL) {
			fprintf(stderr, "Error: database %s doesn't exist\n", (yyvsp[(2) - (3)].string));
		}
	}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 202 "gram.y"
    {
		int ret;
		if ((ret = SM_CreateDatabase(smManager, (yyvsp[(3) - (4)].string))) != NORMAL) {
			fprintf(stderr, "Error: database %s already exists\n", (yyvsp[(3) - (4)].string));
		}
	}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 211 "gram.y"
    {
		int ret;
		if ((ret = SM_DropDatabase(smManager, (yyvsp[(3) - (4)].string))) != NORMAL) {
			if (ret == SM_NODIR)
				fprintf(stderr, "Error: database %s doesn't exist\n", (yyvsp[(3) - (4)].string));
		}
	}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 221 "gram.y"
    {
		int ret;
		if ((ret = SM_CreateTable(smManager, (yyvsp[(3) - (7)].string), (yyvsp[(5) - (7)].attrInfo))) != NORMAL) {
			if (ret == SM_NODBSELECTED)
				fprintf(stderr, "Error: no database selected\n");
			else if (ret == SM_DUPLICATEATTR)
				fprintf(stderr, "Error: duplicate column name\n");
			else if (ret == PF_EXIST)
				fprintf(stderr, "Error: table %s already exists\n", (yyvsp[(3) - (7)].string));
			else if (ret == SM_VIEWEXIST)
				fprintf(stderr, "Error: view %s already exists\n", (yyvsp[(3) - (7)].string));
		}
    }
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 237 "gram.y"
    {
		int ret;
		if ((ret = SM_DropTable(smManager, (yyvsp[(3) - (4)].string))) != NORMAL) {
			if (ret == SM_NODBSELECTED)
				fprintf(stderr, "Error: no database selected\n");
			else if (ret == PF_NOTEXIST)
				fprintf(stderr, "Error: table %s doesn't exist\n", (yyvsp[(3) - (4)].string));
		}
    }
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 249 "gram.y"
    {
		int ret;
        if ((ret = SM_CreateView(smManager, (yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].expression))) != NORMAL) {
        	if (ret == SM_NODBSELECTED)
				fprintf(stderr, "Error: no database selected\n");
			else if (ret == SM_VIEWEXIST)
				fprintf(stderr, "Error: view %s already exists\n", (yyvsp[(3) - (6)].string));
			else if (ret == SM_TABLEEXIST)
				fprintf(stderr, "Error: table %s already exists\n", (yyvsp[(3) - (6)].string));
		}
    }
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 263 "gram.y"
    {
		int ret;
        if ((ret = SM_DropView(smManager, (yyvsp[(3) - (4)].string))) != NORMAL) {
        	if (ret == SM_NODBSELECTED)
				fprintf(stderr, "Error: no database selected\n");
			else if (ret == SM_VIEWNOTEXIST)
				fprintf(stderr, "Error: view %s doesn't exist\n", (yyvsp[(3) - (4)].string));
		}
    }
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 275 "gram.y"
    {
		(yyval.attrInfo) = (yyvsp[(1) - (1)].attrInfo);
	}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 278 "gram.y"
    {
		AttrInfo *p = GET_LAST(AttrInfo, (yyvsp[(1) - (3)].attrInfo), next);
		p->next = (yyvsp[(3) - (3)].attrInfo);
		(yyval.attrInfo) = (yyvsp[(1) - (3)].attrInfo);
    }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 285 "gram.y"
    {
		(yyval.attrInfo) = NEW(AttrInfo);
        (yyval.attrInfo)->name = (yyvsp[(1) - (5)].string);
        (yyval.attrInfo)->type = (yyvsp[(2) - (5)].attrType);
        switch ((yyvsp[(2) - (5)].attrType)) {
        case INT: (yyval.attrInfo)->size = INT_SIZE; break;
        case STRING: (yyval.attrInfo)->size = MAXSTRINGLEN; break;
        case FLOAT: (yyval.attrInfo)->size = FLOAT_SIZE; break;
        }
		(yyval.attrInfo)->isPrimaryKey = (yyvsp[(3) - (5)].number);
		(yyval.attrInfo)->foriegnKey = (yyvsp[(4) - (5)].relAttr);
		(yyval.attrInfo)->check = (yyvsp[(5) - (5)].checkCondition);
    }
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 300 "gram.y"
    { (yyval.number) = 0; }
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 301 "gram.y"
    {
		(yyval.number) = 1;
	}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 307 "gram.y"
    { (yyval.relAttr) = NULL; }
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 308 "gram.y"
    {
		(yyval.relAttr) = NEW(RelAttr);
		(yyval.relAttr)->relName = (yyvsp[(4) - (7)].string);
		(yyval.relAttr)->attrName = (yyvsp[(6) - (7)].string);
	}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 316 "gram.y"
    { (yyval.checkCondition) = NULL; }
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 317 "gram.y"
    {
		(yyval.checkCondition) = NEW(CheckCondition);
		(yyval.checkCondition)->attr = (yyvsp[(3) - (6)].string);
		(yyval.checkCondition)->op = (yyvsp[(4) - (6)].compOp);
		(yyval.checkCondition)->value = (yyvsp[(5) - (6)].value);
	}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 326 "gram.y"
    {
        (yyval.attrType) = INT;
    }
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 329 "gram.y"
    {
        (yyval.attrType) = STRING;
    }
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 332 "gram.y"
    {
        (yyval.attrType) = FLOAT;
    }
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 337 "gram.y"
    {
		int ret;
		if ((ret = QL_Insert(qlManager, (yyvsp[(3) - (8)].string), (yyvsp[(6) - (8)].valueList))) != NORMAL) {
			if (ret == QL_DUPLICATE)
				fprintf(stderr, "Error: duplicate primary key\n");
			else if (ret == QL_CHECKFAIL)
				fprintf(stderr, "Error: check failed\n");
			else if (ret == QL_FOREIGNNOFOUND)
				fprintf(stderr, "Error: foreign key no found\n");
			else if (ret == QL_WRONGVALUENUM)
				fprintf(stderr, "Error: wrong value number\n");
			else if (ret == QL_WRONGTYPE)
				fprintf(stderr, "Error: type doesn't match\n");
		}
    }
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 355 "gram.y"
    {
		(yyval.valueList) = NEW(ValueList);
		(yyval.valueList)->v = (yyvsp[(1) - (1)].value);
		(yyval.valueList)->next = NULL;
	}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 360 "gram.y"
    {
        ValueList *p = GET_LAST(ValueList, (yyvsp[(1) - (3)].valueList), next);
		p->next = NEW(ValueList);
		p->next->v = (yyvsp[(3) - (3)].value);
		p->next->next = NULL;
		(yyval.valueList) = (yyvsp[(1) - (3)].valueList);
    }
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 370 "gram.y"
    {
        (yyval.value) = NEW(Value);
        (yyval.value)->type = INT;
        int *p = malloc(sizeof(int));
        *p = (yyvsp[(1) - (1)].number);
        (yyval.value)->data = p;
    }
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 377 "gram.y"
    {
        (yyval.value) = NEW(Value);
        (yyval.value)->type = STRING;
		(yyval.value)->data = malloc(STRING_SIZE);
		strncpy((yyval.value)->data, (yyvsp[(1) - (1)].string), MAXSTRINGLEN);
		free((yyvsp[(1) - (1)].string));
		((char *)((yyval.value)->data))[MAXSTRINGLEN] = 0;
    }
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 385 "gram.y"
    {
		//TODO
    }
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 391 "gram.y"
    {
		IDList *il = NEW(IDList);
		il->id = (yyvsp[(3) - (6)].string);
		Expression *exp = translateQuery(NULL, il, (yyvsp[(5) - (6)].condition));
		int ret;
        if ((ret = QL_Delete(qlManager, (yyvsp[(3) - (6)].string), exp)) != NORMAL) {
			if (ret == QL_ONLYFK)
				fprintf(stderr, "can't delete foreign key\n");
		}
    }
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 401 "gram.y"
    {
		IDList *il = NEW(IDList);
		il->id = (yyvsp[(3) - (4)].string);
		Expression *exp = translateQuery(NULL, il, NULL);
		int ret;
        if ((ret = QL_Delete(qlManager, (yyvsp[(3) - (4)].string), exp)) != NORMAL) {
			if (ret == QL_ONLYFK)
				fprintf(stderr, "can't delete foreign key\n");
		}
    }
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 411 "gram.y"
    {
		IDList *il = NEW(IDList);
		il->id = (yyvsp[(4) - (5)].string);
		Expression *exp = translateQuery(NULL, il, NULL);
		int ret;
        if ((ret = QL_Delete(qlManager, (yyvsp[(4) - (5)].string), exp)) != NORMAL) {
			if (ret == QL_ONLYFK)
				fprintf(stderr, "can't delete foreign key\n");
		}
    }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 424 "gram.y"
    {
		int ret;
		IDList *il = NEW(IDList);
		il->id = (yyvsp[(2) - (7)].string);
		Expression *exp = translateQuery(NULL, il, (yyvsp[(6) - (7)].condition));
		if ((ret = QL_Update(qlManager, (yyvsp[(2) - (7)].string), (yyvsp[(4) - (7)].assignmentList), exp)) != NORMAL) {
			if (ret == QL_DUPLICATE)
				fprintf(stderr, "Error: duplicate primary key\n");
			else if (ret == QL_CHECKFAIL)
				fprintf(stderr, "Error: check failed\n");
			else if (ret == QL_ONLYFK)
				fprintf(stderr, "can't update foreign key\n");
		}
	}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 441 "gram.y"
    {
		(yyval.assignmentList) = NEW(AssignmentList);
		(yyval.assignmentList)->left = (yyvsp[(1) - (3)].relAttr);
		(yyval.assignmentList)->right = (yyvsp[(3) - (3)].relAttrValue);
		(yyval.assignmentList)->next = NULL;
	}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 447 "gram.y"
    {
		AssignmentList *p = GET_LAST(AssignmentList, (yyvsp[(1) - (5)].assignmentList), next);
		p->next = NEW(AssignmentList);
		p->next->left = (yyvsp[(3) - (5)].relAttr);
		p->next->right = (yyvsp[(5) - (5)].relAttrValue);
		p->next->next = NULL;
	}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 457 "gram.y"
    {
		(yyval.expression) = translateQuery((yyvsp[(2) - (6)].relAttrList), (yyvsp[(4) - (6)].idList), (yyvsp[(6) - (6)].condition));
	}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 460 "gram.y"
    {
		(yyval.expression) = translateQuery((yyvsp[(2) - (4)].relAttrList), (yyvsp[(4) - (4)].idList), NULL);
	}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 466 "gram.y"
    {
		int ret;
		if ((ret = QL_Select(qlManager, (yyvsp[(1) - (2)].expression))) != NORMAL) {
		}
    }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 474 "gram.y"
    {
		(yyval.relAttrList) = NULL;
	}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 477 "gram.y"
    {
        (yyval.relAttrList) = (yyvsp[(1) - (1)].relAttrList);
    }
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 483 "gram.y"
    {
        (yyval.condition) = NEW(Condition);
		(yyval.condition)->kind = CompOpCond;
		(yyval.condition)->u.coc = NEW(CompOpCondition);
        (yyval.condition)->u.coc->left = (yyvsp[(1) - (3)].relAttrValue);
        (yyval.condition)->u.coc->op = (yyvsp[(2) - (3)].compOp);
        (yyval.condition)->u.coc->right = (yyvsp[(3) - (3)].relAttrValue);
    }
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 491 "gram.y"
    {
        (yyval.condition) = NEW(Condition);
		(yyval.condition)->kind = InCond;
		(yyval.condition)->u.ic = NEW(InCondition);
		(yyval.condition)->u.ic->avl = (yyvsp[(1) - (5)].relAttrValueList);
		(yyval.condition)->u.ic->rel = (yyvsp[(4) - (5)].expression);
	}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 498 "gram.y"
    {
        (yyval.condition) = NEW(Condition);
		(yyval.condition)->kind = AndCond;
		(yyval.condition)->u.ac = NEW(AndCondition);
		(yyval.condition)->u.ac->left = (yyvsp[(1) - (3)].condition);
		(yyval.condition)->u.ac->right = (yyvsp[(3) - (3)].condition);
	}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 505 "gram.y"
    {
        (yyval.condition) = NEW(Condition);
		(yyval.condition)->kind = OrCond;
		(yyval.condition)->u.oc = NEW(OrCondition);
		(yyval.condition)->u.oc->left = (yyvsp[(1) - (3)].condition);
		(yyval.condition)->u.oc->right = (yyvsp[(3) - (3)].condition);
	}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 512 "gram.y"
    {
        (yyval.condition) = NEW(Condition);
		(yyval.condition)->kind = NotCond;
		(yyval.condition)->u.nc = NEW(NotCondition);
		(yyval.condition)->u.nc->cond = (yyvsp[(2) - (2)].condition);
	}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 518 "gram.y"
    {
		(yyval.condition) = (yyvsp[(2) - (3)].condition);
	}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 524 "gram.y"
    { (yyval.compOp) = EQ_OP; }
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 525 "gram.y"
    { (yyval.compOp) = LT_OP; }
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 526 "gram.y"
    { (yyval.compOp) = GT_OP; }
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 527 "gram.y"
    { (yyval.compOp) = LE_OP; }
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 528 "gram.y"
    { (yyval.compOp) = GE_OP; }
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 529 "gram.y"
    { (yyval.compOp) = NE_OP; }
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 533 "gram.y"
    {
		(yyval.relAttrValue) = NEW(RelAttrValue);
		(yyval.relAttrValue)->isValue = 0;
		(yyval.relAttrValue)->u.a = (yyvsp[(1) - (1)].relAttr);
    }
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 538 "gram.y"
    {
		(yyval.relAttrValue) = NEW(RelAttrValue);
		(yyval.relAttrValue)->isValue = 1;
		(yyval.relAttrValue)->u.v = (yyvsp[(1) - (1)].value);
    }
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 545 "gram.y"
    {
		(yyval.relAttrValueList) = NEW(RelAttrValueList);
		(yyval.relAttrValueList)->av = (yyvsp[(1) - (1)].relAttrValue);
		(yyval.relAttrValueList)->next = NULL;
	}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 550 "gram.y"
    {
        RelAttrValueList *p = GET_LAST(RelAttrValueList, (yyvsp[(1) - (3)].relAttrValueList), next);
		p->next = NEW(RelAttrList);
		p->next->av = (yyvsp[(3) - (3)].relAttrValue);
		p->next->next = NULL;
		(yyval.relAttrValueList) = (yyvsp[(1) - (3)].relAttrValueList);
	}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 560 "gram.y"
    {
		(yyval.relAttr) = NEW(RelAttr);
        (yyval.relAttr)->relName = NULL;
        (yyval.relAttr)->attrName = (yyvsp[(1) - (1)].string);
    }
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 565 "gram.y"
    {
		(yyval.relAttr) = NEW(RelAttr);
        (yyval.relAttr)->relName = (yyvsp[(1) - (3)].string);
        (yyval.relAttr)->attrName = (yyvsp[(3) - (3)].string);
    }
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 572 "gram.y"
    {
		(yyval.relAttrList) = NEW(RelAttrList);
		(yyval.relAttrList)->a = (yyvsp[(1) - (1)].relAttr);
		(yyval.relAttrList)->next = NULL;
	}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 577 "gram.y"
    {
        RelAttrList *p = GET_LAST(RelAttrList, (yyvsp[(1) - (3)].relAttrList), next);
		p->next = NEW(RelAttrList);
		p->next->a = (yyvsp[(3) - (3)].relAttr);
		p->next->next = NULL;
		(yyval.relAttrList) = (yyvsp[(1) - (3)].relAttrList);
    }
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 586 "gram.y"
    {
		(yyval.idList) = NEW(IDList);
		(yyval.idList)->id = (yyvsp[(1) - (1)].string);
		(yyval.idList)->next = NULL;
	}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 591 "gram.y"
    {
		IDList *p = GET_LAST(IDList, (yyvsp[(1) - (3)].idList), next);
		p->next = NEW(IDList);
		p->next->id = (yyvsp[(3) - (3)].string);
		p->next->next = NULL;
		(yyval.idList) = (yyvsp[(1) - (3)].idList);
    }
    break;



/* Line 1455 of yacc.c  */
#line 2421 "gram.c"
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



/* Line 1675 of yacc.c  */
#line 599 "gram.y"


int sbp_parse(PF_Manager *pfm, SM_Manager *smm, QL_Manager *qlm) {
    pfManager = pfm;
    smManager = smm;
    qlManager = qlm;
    return yyparse();
}

