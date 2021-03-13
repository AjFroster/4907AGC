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

	v->type = INSTRUCTIONS;

	// first instruction
	if(instructions == NULL) {
		instructions = (AST_Node**) malloc (sizeof (AST_Node));
		instructions[0] = instruction;
		instruction_count = 1;
	}
	// add a new instruction
	else {
		instructions = (AST_Node**) realloc (instructions, (instruction_count + 1) * sizeof (AST_Node*));
		instructions[instruction_count] = instruction;
		instruction_count++;
	}

	v->instructions = instructions;
	v->instruction_count = instruction_count;

	return (struct AST_Node *) v;
}

AST_Node *new_ast_datum_node(list_t *entry) {
	AST_Node_Datum *v = malloc (sizeof (AST_Node_Datum));

	v->type = DATUM_NODE;
	v->name = entry->st_name;

	return (struct AST_Node *) v;
}

AST_Node *new_ast_const_node(list_t *entry, int val) {
	AST_Node_Const *v = malloc (sizeof (AST_Node_Const));
	//list_t l;

	v->type = CONST_NODE;
	v->name = entry->st_name;
	v->val = val;

	if(lookup(entry->st_name) == NULL) {
		printf("Error Creating Constant Node");
		return NULL;
	}

	return (struct AST_Node *) v;
}

AST_Node *new_ast_input_node(list_t *entry) {
	AST_Node_Input *v = malloc (sizeof (AST_Node_Input));

	v->type = INPUT_NODE;
	v->name = entry->st_name;

	return (struct AST_Node *) v;
}

AST_Node *new_ast_output_node(list_t *entry) {
	AST_Node_Output *v = malloc (sizeof (AST_Node_Output));

	v->type = OUTPUT_NODE;
	v->name = entry->st_name;

	return (struct AST_Node *) v;
}

/* AST Traversal */

void ast_print_node(AST_Node *node) {
	AST_Node_Instructions *temp_instructions;
	AST_Node_Datum *temp_datum;
	AST_Node_Const *temp_const;
	AST_Node_Input *temp_input;
	AST_Node_Output *temp_output;

	//printf("Node: type: %d\n", node->type);

	switch(node->type) {
		case NODE:
			printf("Basic Node\n");
			break;
		case INSTRUCTIONS:
			temp_instructions = (struct AST_Node_Instructions *) node;
			printf("Instruction node of type: %d\n", temp_instructions->instructions[0]->type);
			break;
		case DATUM_NODE:
			temp_datum = (struct AST_Node_Datum *) node;
			printf("Datum Node with id: %s\n", temp_datum->name);
			break;
		case CONST_NODE:
			temp_const = (struct AST_Node_Const *) node;
			printf("Constant node with id: %s and value: %d\n", temp_const->name, temp_const->val);
			break;
		case OUTPUT_NODE:
			temp_output = (struct AST_Node_Output *) node;
			printf("Output node with id: %s\n", temp_output->name);
			break;
		case INPUT_NODE:
			temp_input = (struct AST_Node_Input *) node;
			printf("Input node with id: %s\n", temp_input->name);
			break;
		default:
			fprintf(stderr, "Error has occured\n");
			exit(1);
	}
	printf("ANOTHER TEST\n");
}

void ast_traversal(AST_Node *node) {
	int i;
	//printf("TESTINGTESTINGTESTING\n");
	
	//printf("Node: type: %d %d\n", node->type, OUTPUT_NODE);

	if(node == NULL) {
		printf("no children\n");
		return;
	}


	if(node->type == NODE) {
		//printf("TESTING\n");
		ast_traversal(node->left);
		ast_traversal(node->right);
		ast_print_node(node);
	}
	
	// instructions case
	else if(node->type == INSTRUCTIONS) {
		//printf("TESTING\n");
		//printf("Node: type: %d\n", node->type);
		AST_Node_Instructions *temp_instructions = (struct AST_Node_Instructions *) node;
		ast_print_node(node);
		for(i = 0; i < temp_instructions->instruction_count; i++) {
			//printf("value i: %d\n", i);
			ast_traversal(temp_instructions->instructions[i]);
		}
	}
	else if(node->type == NODE || node->type == DATUM_NODE || node->type == CONST_NODE || node->type == INPUT_NODE || node->type == OUTPUT_NODE) {
		//printf("Node: type: %d\n", node->type);
		//ast_traversal(node->left);
		//ast_traversal(node->right);
		//printf("testing12345\n");
		ast_print_node(node);
	}
	else {
		//printf("TESTING\n");
		ast_print_node(node);
	}
}



