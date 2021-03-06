%{
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
	char char_val;
	int int_val;
	double double_val;
	char* str_val;

	//structures
	list_t* symtab_item;
	AST_Node* node; //worry about this when implementing AST
}

/* token definition */
%token <node> TOKEN_DATUM TOKEN_INPUT TOKEN_OUTPUT TOKEN_OPERATOR TOKEN_SUBGRAPH TOKEN_CONST TOKEN_EXPANSION TOKEN_EXPAND TOKEN_MAPIN TOKEN_MAPOUT 
%token <int_val> TOKEN_IF TOKEN_ELSE TOKEN_MERGE 
%token <int_val> IOP OOP ADDOP MULOP SUBOP RELOP
%token <int_val> LPAR RPAR LBRACK RBRACK LBRACE RBRACE SEMI DOT COMMA
%token <symtab_item> ID
%token <int_val> ICONST FCONST CCONST 
%token <str_val> STRING

/* precedencies and associativities */
%left LPAREN RPAREN LBRACK RBRACK
%right NOTOP INCR REFER
%left MULOP DIVOP
%left ADDOP
%left RELOP
%left EQUOP
%left OROP
%left ANDOP
%right ASSIGN
%left COMMA

/* rule (non-terminal) definition */
//type <node> program
//type <node> subgraph
%type <node> instructions instruction

%start program

/* expression rules */

%%

program: subgraphs;

subgraphs: subgraph | subgraphs subgraph;

subgraph: {declare = 0;} TOKEN_SUBGRAPH LPAR ID RPAR {declare = 1;} instructions;

instructions: instruction 
		{
			$$ = new_instructions_node(NULL, 0, $1);		
		}
		| instructions instruction
		{
			AST_Node_Instructions *temp = (AST_Node_Instructions*) $1;
			$$ = new_instructions_node(temp->instructions, temp->instruction_count, $2);
		};

instruction: TOKEN_DATUM LPAR ID RPAR SEMI
				| TOKEN_OUTPUT LPAR ID RPAR SEMI
				| TOKEN_INPUT LPAR ID RPAR SEMI
				| TOKEN_CONST LPAR ID COMMA ICONST RPAR SEMI
				| TOKEN_OPERATOR LPAR ID COMMA RELOP COMMA ID COMMA ID RPAR SEMI
				| TOKEN_OPERATOR LPAR ID COMMA TOKEN_IF COMMA ID COMMA ID RPAR SEMI
				| TOKEN_OPERATOR LPAR ID COMMA TOKEN_ELSE COMMA ID COMMA ID RPAR SEMI
				| TOKEN_OPERATOR LPAR ID COMMA MULOP COMMA ID COMMA ID RPAR SEMI
				| TOKEN_OPERATOR LPAR ID COMMA ADDOP COMMA ID COMMA ID RPAR SEMI
				| TOKEN_OPERATOR LPAR ID COMMA SUBOP COMMA ID COMMA ID RPAR SEMI
				| TOKEN_OPERATOR LPAR ID COMMA TOKEN_MERGE COMMA ID COMMA ID RPAR SEMI
				| TOKEN_EXPAND LPAR ID COMMA TOKEN_MAPIN LPAR ID COMMA ID RPAR SEMI 
				| TOKEN_MAPOUT LPAR ID COMMA ID RPAR SEMI RPAR SEMI 
				| TOKEN_MAPIN LPAR ID COMMA ID RPAR SEMI RPAR SEMI 
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
		printf("Warning: Element(s) of the revisit queue have been left unchecked");
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
