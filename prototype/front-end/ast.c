#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------AST NODE MANAGEMENT-------------------- */
/* The basic node */
AST_Node *new_ast_node(Node_Type type, AST_Node *left, AST_Node *right){
	// allocate memory
	AST_Node *v = malloc (sizeof (AST_Node));
	
	// set entries
	v->type = type;
	v->left = left;
	v->right = right;
	
	// return the result
	return v;
}

AST_Node *new_instructions_node(AST_Node **instructions, int instruction_count, AST_Node *instruction){
	// allocate memory
	AST_Node_Instructions *v = malloc (sizeof (AST_Node_Instructions));
	
}
