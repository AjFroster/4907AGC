/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
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
    TOKEN_DATUM = 258,
    TOKEN_INPUT = 259,
    TOKEN_OUTPUT = 260,
    TOKEN_OPERATOR = 261,
    TOKEN_SUBGRAPH = 262,
    TOKEN_CONST = 263,
    TOKEN_EXPANSION = 264,
    TOKEN_EXPAND = 265,
    TOKEN_MAPIN = 266,
    TOKEN_MAPOUT = 267,
    TOKEN_IF = 268,
    TOKEN_ELSE = 269,
    TOKEN_MERGE = 270,
    IOP = 271,
    OOP = 272,
    ADDOP = 273,
    MULOP = 274,
    SUBOP = 275,
    RELOP = 276,
    LPAR = 277,
    RPAR = 278,
    LBRACK = 279,
    RBRACK = 280,
    LBRACE = 281,
    RBRACE = 282,
    SEMI = 283,
    DOT = 284,
    COMMA = 285,
    ID = 286,
    ICONST = 287,
    FCONST = 288,
    CCONST = 289,
    STRING = 290,
    LPAREN = 291,
    RPAREN = 292,
    NOTOP = 293,
    INCR = 294,
    REFER = 295,
    DIVOP = 296,
    EQUOP = 297,
    OROP = 298,
    ANDOP = 299,
    ASSIGN = 300
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 15 "parser.y"

	char char_val;
	int int_val;
	double double_val;
	char* str_val;

	//structures
	list_t* symtab_item;
	AST_Node* node; //worry about this when implementing AST

#line 114 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
