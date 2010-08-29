
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

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 179 "gram.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


