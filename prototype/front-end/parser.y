%{
	#include "ast.h"
	#include "ast.c"
	#include "symboltable.c"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	extern FILE *yyin;
	extern FILE *yyout;
	extern int lineno;
	extern int yylex();
	void yyerror();
%}

/* YYSTYPE union */
%union{
	Value val;

	/*
	char char_val;
	int int_val;
	double double_val;
	char* str_val;
	*/

	//structures
	list_t* symtab_item;
	AST_Node* node;

	Param par;
}

/* token definition */
%token<val> TOKEN_DATUM TOKEN_INPUT TOKEN_OUTPUT TOKEN_OPERATOR TOKEN_SUBGRAPH TOKEN_CONST
%token<val> TOKEN_IF TOKEN_ELSE TOKEN_MERGE TOKEN_EXPANSION TOKEN_EXPAND TOKEN_MAPIN TOKEN_MAPOUT 
%token <val> IOP OOP ADDOP MULOP SUBOP RELOP
%token <val> LPAR RPAR LBRACK RBRACK LBRACE RBRACE SEMI DOT COMMA
%token <symtab_item> ID
%token <val> ICONST FCONST CCONST 
%token <val> STRING

/* precedencies and associativities */
%left COMMA
%right ASSIGN
%left OROP
%left ANDOP
%left EQUOP
%left RELOP
%left ADDOP
%left MULOP DIVOP
%right NOTOP INCR REFER MINUS
%left LPAREN RPAREN LBRACK RBRACK

/* rule (non-terminal) definition */
%type <node> program
%type <node> subgraphs subgraph
%type <node> instructions instruction


%start program

/* expression rules */

%%

program: subgraphs;

subgraphs: subgraph | subgraphs subgraph;

subgraph: {declare = 0;} TOKEN_SUBGRAPH LPAR ID RPAR {declare = 1; incr_scope();} instructions
				{ hide_scope(); } ;

instructions: instruction {ast_traversal($1);} | instructions instruction;

instruction: TOKEN_DATUM LPAR ID RPAR SEMI
				{
					$$ = new_ast_datum_node($3);
				}
				//| TOKEN_OUTPUT LPAR ID RPAR SEMI
				//| TOKEN_INPUT LPAR ID RPAR SEMI
				| TOKEN_CONST LPAR ID COMMA ICONST RPAR SEMI
				{
					$$ = new_ast_const_node($3, $5);
				}
				//| TOKEN_OPERATOR LPAR ID COMMA RELOP COMMA ID COMMA ID RPAR SEMI
				//| TOKEN_OPERATOR LPAR ID COMMA TOKEN_IF COMMA ID COMMA ID RPAR SEMI
				//| TOKEN_OPERATOR LPAR ID COMMA TOKEN_ELSE COMMA ID COMMA ID RPAR SEMI
				//| TOKEN_OPERATOR LPAR ID COMMA MULOP COMMA ID COMMA ID RPAR SEMI
				//| TOKEN_OPERATOR LPAR ID COMMA ADDOP COMMA ID COMMA ID RPAR SEMI
				//| TOKEN_OPERATOR LPAR ID COMMA SUBOP COMMA ID COMMA ID RPAR SEMI
				//| TOKEN_OPERATOR LPAR ID COMMA TOKEN_MERGE COMMA ID COMMA ID RPAR SEMI
				//| TOKEN_EXPAND LPAR ID COMMA TOKEN_MAPIN LPAR ID COMMA ID RPAR SEMI 
				//| TOKEN_MAPOUT LPAR ID COMMA ID RPAR SEMI RPAR SEMI 
				//| TOKEN_MAPIN LPAR ID COMMA ID RPAR SEMI RPAR SEMI 
				;

%%

void yyerror ()
{
  fprintf(stderr, "Syntax error at line %d\n", lineno);
  exit(1);
}

int main (int argc, char *argv[]){

	// initialize symbol table
	init_hash_table();

	// initialize revisit queue
	queue = NULL;

	// parsing
	int flag;
	yyin = fopen(argv[1], "r");
	flag = yyparse();
	fclose(yyin);
	
	printf("Parsing finished!\n");

	if(queue != NULL) {
		printf("Warning: Element(s) of the revisit queue have been left unchecked\n");
	}
	
	// symbol table dump
	yyout = fopen("symtab_dump.out", "w");
	symtab_dump(yyout);
	fclose(yyout);

	// revisit queue dump
	yyout = fopen("revisit_dump.out", "w");
	revisit_dump(yyout);
	fclose(yyout);

	return flag;
}
