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
	//Value val;

	
	char char_val;
	int int_val;
	double double_val;
	char* str_val;
	

	//structures
	list_t* symtab_item;
	AST_Node* node;

	Param par;
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
//%type <node> datum
//%type <node> operation



%start program

/* expression rules */

%%

program: subgraphs {ast_traversal($1);};


subgraphs: subgraph | subgraphs subgraph;

subgraph: /*{declare = 0;}*/ TOKEN_SUBGRAPH LPAR ID RPAR {/*declare = 1;*/ incr_scope();} instructions 	
				/*{ hide_scope(); } */;

instructions: instruction 
		{
			$$ = new_instructions_node(NULL, 0, $1);
			//printf("%d\n", $1->type);
			//ast_traversal($$); //just for testing		
		}
		| instructions instruction
		{
			AST_Node_Instructions *temp = (AST_Node_Instructions*) $1;
			$$ = new_instructions_node(temp->instructions, temp->instruction_count, $2);
			//ast_traversal($$); //just for testing
		};

instruction: TOKEN_DATUM LPAR ID RPAR SEMI
				{
					$$ = new_ast_datum_node($3);
					//AST_Node_Datum *temp = (AST_Node_Datum*) $$;
					ast_traversal($$); //just for testing
				}
				| TOKEN_OUTPUT LPAR ID RPAR SEMI
				{
					$$ = new_ast_output_node($3);
					//printf("%d %s\n", $$->type, $3->st_name);
					//AST_Node_Datum *temp = (AST_Node_Datum*) $$;
					ast_traversal($$); //just for testing
				}
				| TOKEN_INPUT LPAR ID RPAR SEMI
				{
					$$ = new_ast_input_node($3);
					//printf("%d %s\n", $$->type, $3->st_name);
					//AST_Node_Datum *temp = (AST_Node_Datum*) $$;
					ast_traversal($$); //just for testing
				}
				| TOKEN_CONST LPAR ID COMMA ICONST RPAR SEMI
				{
					$$ = new_ast_const_node($3, $5);
					//AST_Node_Const *temp = (AST_Node_Const*) $$;
					ast_traversal($$); //just for testing
				}
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


	/*
instruction: TOKEN_DATUM LPAR ID RPAR SEMI
				{
					$$ = new_ast_datum_node($3);
					//AST_Node_Datum *temp = (AST_Node_Datum*) $$;
					ast_traversal($$); //just for testing
				}
				| TOKEN_OUTPUT LPAR ID RPAR SEMI
				| TOKEN_INPUT LPAR ID RPAR SEMI
				| TOKEN_CONST LPAR ID COMMA ICONST RPAR SEMI
				{
					$$ = new_ast_const_node($3, $5);
					//AST_Node_Const *temp = (AST_Node_Const*) $$;
					ast_traversal($$); //just for testing
				}
				//| TOKEN_OPERATOR LPAR ID COMMA RELOP COMMA ID COMMA ID RPAR SEMI
				//| TOKEN_OPERATOR LPAR ID COMMA TOKEN_IF COMMA ID COMMA ID RPAR SEMI
				//| TOKEN_OPERATOR LPAR ID COMMA TOKEN_ELSE COMMA ID COMMA ID RPAR SEMI
				| TOKEN_OPERATOR LPAR ID COMMA MULOP COMMA ID COMMA ID RPAR SEMI
				//| TOKEN_OPERATOR LPAR ID COMMA ADDOP COMMA ID COMMA ID RPAR SEMI
				//| TOKEN_OPERATOR LPAR ID COMMA SUBOP COMMA ID COMMA ID RPAR SEMI
				//| TOKEN_OPERATOR LPAR ID COMMA TOKEN_MERGE COMMA ID COMMA ID RPAR SEMI
				//| TOKEN_EXPAND LPAR ID COMMA TOKEN_MAPIN LPAR ID COMMA ID RPAR SEMI 
				//| TOKEN_MAPOUT LPAR ID COMMA ID RPAR SEMI RPAR SEMI 
				//| TOKEN_MAPIN LPAR ID COMMA ID RPAR SEMI RPAR SEMI 
				;

				atum: TOKEN_DATUM LPAR ID RPAR SEMI
		{
			$$ = new_ast_datum_node($3);
			//printf("%d %s\n", $$->type, $3->st_name);
			//AST_Node_Datum *temp = (AST_Node_Datum*) $$;
			//ast_traversal($$); //just for testing
		}
		| TOKEN_OUTPUT LPAR ID RPAR SEMI
		{
			$$ = new_ast_output_node($3);
			//printf("%d %s\n", $$->type, $3->st_name);
			//AST_Node_Datum *temp = (AST_Node_Datum*) $$;
			//ast_traversal($$); //just for testing
		}
		| TOKEN_INPUT LPAR ID RPAR SEMI
		{
			$$ = new_ast_input_node($3);
			//printf("%d %s\n", $$->type, $3->st_name);
			//AST_Node_Datum *temp = (AST_Node_Datum*) $$;
			//ast_traversal($$); //just for testing
		}
		| TOKEN_CONST LPAR ID COMMA ICONST RPAR SEMI
		{
			$$ = new_ast_const_node($3, $5);
			//AST_Node_Const *temp = (AST_Node_Const*) $$;
			//ast_traversal($$); //just for testing
		}
		;

operation: TOKEN_OPERATOR LPAR ID COMMA RELOP COMMA ID COMMA ID RPAR SEMI
			//| TOKEN_OPERATOR LPAR ID COMMA TOKEN_IF COMMA ID COMMA ID RPAR SEMI
			//| TOKEN_OPERATOR LPAR ID COMMA TOKEN_ELSE COMMA ID COMMA ID RPAR SEMI
			| TOKEN_OPERATOR LPAR ID COMMA MULOP COMMA ID COMMA ID RPAR SEMI
			//| TOKEN_OPERATOR LPAR ID COMMA ADDOP COMMA ID COMMA ID RPAR SEMI
			//| TOKEN_OPERATOR LPAR ID COMMA SUBOP COMMA ID COMMA ID RPAR SEMI
			//| TOKEN_OPERATOR LPAR ID COMMA TOKEN_MERGE COMMA ID COMMA ID RPAR SEMI
			//| TOKEN_EXPAND LPAR ID COMMA TOKEN_MAPIN LPAR ID COMMA ID RPAR SEMI 
			//| TOKEN_MAPOUT LPAR ID COMMA ID RPAR SEMI RPAR SEMI 
			//| TOKEN_MAPIN LPAR ID COMMA ID RPAR SEMI RPAR SEMI 
			;
*/
}
