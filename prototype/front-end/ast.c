#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* AST NODE MANAGEMENT */

AST_Node *new_ast_node(Node_Type type, AST_Node *left, AST_Node *right) {
	// allocate memory for new node
	AST_Node *v = malloc (sizeof (AST_Node));

	v->type;
	v->left = left;
	v->right = right;

	return v;
}

AST_Node *new_instructions_node(AST_Node **instructions, int instruction_count, AST_Node *instruction){
	// allocate memory
	AST_Node_Instructions *v = malloc (sizeof (AST_Node_Instructions));
	
}

AST_Node *new_ast_datum_node(char *name) {
	AST_Node_Datum *v = malloc (sizeof (AST_Node_Datum));

	v->type = DATUM_NODE;
	v->name = name;

	return (struct AST_Node *) v;
}

AST_Node *new_ast_const_node(char *name, int val) {
	AST_Node_Const *v = malloc (sizeof (AST_Node_Const));
	list_t l;

	v->type = CONST_NODE;
	v->name = name;
	v->val = val;

	if(lookup(v->name == NULL)) {
		printf("Error Creating Constant Node");
		return NULL;
	}

	return (struct AST_Node *) v;
}

/* AST Traversal */

void ast_print_node(AST_Node *node) {
	AST_Node_Datum *temp_datum;
	AST_Node_Const *temp_const;

	switch(node->type) {
		case NODE:
			printf("Basic Node\n");
			break;
		case DATUM_NODE:
			temp_datum = (struct AST_Node_Datum *) node;
			printf("Datum Node\n");
			break;
		case CONST_NODE:
			temp_const = (struct AST_Node_Const *) node;
			printf("Constant node with value %d\n", temp_const->val);
			break;
		default:
			fprintf(stderr, "Error has occured\n");
			exit(1);
	}
}

void ast_traversal(AST_Node *node) {
	int i;

	if(node == NULL) {
		return;
	}

	if(node->type == NODE || node->type == DATUM_NODE || CONST_NODE) {
		ast_traversal(node->left);
		ast_traversal(node->right);
		ast_print_node(node);
	}
}



