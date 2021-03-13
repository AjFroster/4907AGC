#ifndef AST_H
#define AST_H


#include "symboltable.h"


/*------------NODE TYPES-----------------*/

typedef enum Node_Type {
	NODE, // for roots only
	INSTRUCTIONS,
	DATUM_NODE,
	CONST_NODE,
	OPERATOR_NODE,
	INPUT_NODE,
	OUTPUT_NODE,
	SUBGRAPH_NODE,
	EXPAND_NODE,

}Node_Type;

/* Operator Types */
typedef enum Arithm_op{
	ADD,
	SUB,
	MUL,
}Arithm_op;

typedef enum Rel_op{
	GREATER,
	LESS,
	GREATER_EQUAL,
	LESS_EQUAL,
}Rel_op;



/*------------AST Nodes------------*/
//basic node
typedef struct AST_Node {
	enum Node_Type type;
	struct AST_Node *left;
	struct AST_Node *right;
}AST_Node;

typedef struct AST_Node_Instructions {
	enum Node_Type type;

	struct AST_Node **instructions;
	int instruction_count;
}AST_Node_Instructions;

typedef struct AST_Node_Datum {
	enum Node_Type type;

	char *name;
}AST_Node_Datum;

typedef struct AST_Node_Const {
	enum Node_Type type;
	char *name;
	int val;
}AST_Node_Const;


typedef struct AST_Node_Operator {
	enum Node_Type type;


}AST_Node_Operator;

typedef struct AST_Node_Input {
	enum Node_Type type;
	char *name;
	//int val;
}AST_Node_Input;

typedef struct AST_Node_Output{
	enum Node_Type type;
	char *name;

	//int val;
}AST_Node_Output;


/* AST NODE MANAGEMENT */

AST_Node *new_ast_node(Node_Type type, AST_Node *left, AST_Node *right);
AST_Node *new_ast_datum_node(list_t *entry);
AST_Node *new_ast_input_node(list_t *entry);
AST_Node *new_ast_output_node(list_t *entry);
AST_Node *new_ast_const_node(list_t *entry, int val);

/* AST Traversal */
void ast_print_node(AST_Node *node); // print node information
void ast_traversal(AST_Node *node);


/*
typedef struct AST_NODE_DATUM {
	enum Node_Type type;

	int data_type;

	list_t **names;
	int names_count;
}AST_NODE_DATUM;
*/

#endif

