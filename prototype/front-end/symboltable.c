#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboltable.h"

/* current scope */
int cur_scope = 0;

/* flag variable for declaring */ 
int declare = 1; // 1 = declaring variable, 0 = not declaring variable

void init_hash_table() {
	int i;
	hash_table = malloc(SIZE * sizeof(list_t*));
	for(i = 0; i < SIZE; i++) hash_table[i] = NULL;
}

unsigned int hash(char *key) {
	unsigned int hashval = 0;
	for(;*key!='\0';key++) hashval += *key;
	hashval += key[0] % 11 + (key[0] << 3) - key[0];
	return hashval % SIZE;
}

void insert(char *name, int len, int type, int lineno) {
	unsigned int hashval = hash(name);
	list_t *l = hash_table[hashval];

	while ((l != NULL) && (strcmp(name, l->st_name) != 0)) l = l->next;

	/* case: identifier is not yet in the table */
	if (l == NULL) {
		/* Check declare flag */
		if(declare == 1) {
			l = (list_t*) malloc(sizeof(list_t));
			strncpy(l->st_name, name, len);	
			l->st_type = type;
			l->scope = cur_scope;
			l->lines = (RefList*) malloc(sizeof(RefList));
			l->lines->lineno = lineno;
			l->lines->next = NULL;

			/* add to hash table */
			l->next = hash_table[hashval];
			hash_table[hashval] = l; 
			printf("Inserted %s for the first time with linenumber %d!\n", name, lineno); // error checking
		}
		else {
			/* add to check again later */
			l = (list_t*) malloc(sizeof(list_t));
			strncpy(l->st_name, name, len);
			l->st_type = type;
			l->scope = cur_scope;
			l->lines = (RefList*) malloc(sizeof(RefList));
			l->lines->lineno = lineno;
			l->lines->next = NULL;

			/* add to hash table */
			l->next = hash_table[hashval];
			hash_table[hashval] = l; 
			printf("Inserted %s for the first time with linenumber %d!\n", name, lineno); // error checking
		
			add_to_queue(l->st_name);
		}
	}

	/* indentifier is found in table, so just add new line number */
	else{
		RefList *t = l->lines;
		while (t->next != NULL) t = t->next;
		/* add linenumber to reference list */
		t->next = (RefList*) malloc(sizeof(RefList));
		t->next->lineno = lineno;
		t->next->next = NULL;
		printf("Found %s again at line %d!\n", name, lineno);
	}
}

list_t *lookup(char *name){ /* return symbol if found or NULL if not found */
	unsigned int hashval = hash(name);
	list_t *l = hash_table[hashval];
	while ((l != NULL) && (strcmp(name,l->st_name) != 0)) l = l->next;
	return l; // NULL is not found
}

/* print to stdout by default */ 
void symtab_dump(FILE * of){  
  int i;
  fprintf(of,"------------ ------ ------ ------------\n");
  fprintf(of,"Name         Type   Scope   Line Numbers\n");
  fprintf(of,"------------ ------ ------ -------------\n");
  for (i=0; i < SIZE; ++i){ 
	if (hash_table[i] != NULL){ 
		list_t *l = hash_table[i];
		while (l != NULL){ 
			RefList *t = l->lines;
			fprintf(of,"%-12s ",l->st_name);

			fprintf(of,"%-7s","undef"); // if UNDEF or 0
			fprintf(of,"  %d  ",l->scope);

			while (t != NULL){
				fprintf(of,"%4d ",t->lineno);
			t = t->next;
			}
			fprintf(of,"\n");
			l = l->next;
			
			/*
			if (l->st_type == INT_TYPE) fprintf(of,"%-7s","int");
			else if (l->st_type == REAL_TYPE) fprintf(of,"%-7s","real");
			else if (l->st_type == STR_TYPE) fprintf(of,"%-7s","string");
			else if (l->st_type == ARRAY_TYPE){
				fprintf(of,"array of ");
				if (l->inf_type == INT_TYPE) 		   fprintf(of,"%-7s","int");
				else if (l->inf_type  == REAL_TYPE)    fprintf(of,"%-7s","real");
				else if (l->inf_type  == STR_TYPE) 	   fprintf(of,"%-7s","string");
				else fprintf(of,"%-7s","undef");
			}
			else if (l->st_type == FUNCTION_TYPE){
				fprintf(of,"%-7s %s","function returns ");
				if (l->inf_type == INT_TYPE) 		   fprintf(of,"%-7s","int");
				else if (l->inf_type  == REAL_TYPE)    fprintf(of,"%-7s","real");
				else if (l->inf_type  == STR_TYPE) 	   fprintf(of,"%-7s","string");
				else fprintf(of,"%-7s","undef");
			}
			else fprintf(of,"%-7s","undef"); // if UNDEF or 0
			while (t != NULL){
				fprintf(of,"%4d ",t->lineno);
			t = t->next;
			}
			fprintf(of,"\n");
			l = l->next;
			*/
		}
    }
  }
}


void hide_scope() {
	list_t *l;
	int i;
	printf("Hiding scope \'%d':\n", cur_scope);
	/* for all the lists */
	for (i = 0; i < SIZE; i++) {
		if (hash_table[i] != NULL) {
			l = hash_table[i];
			/* find the first item that is from another scope */
			while (l != NULL && l->scope == cur_scope) {
				printf("Hiding %s..\n", l->st_name);
				l = l->next;
			}
			/* set the list equal to the item */
			hash_table[i] = l;
		}
	}
	cur_scope--;
}

void incr_scope() {
	cur_scope++;
}

Param def_param(char *param_name) {
	Param param;

	/* set the information */
	strcpy(param.par_name, param_name);

	return param;
}

int supgraph_declare(char *name, Param *parameter) {
	list_t *l = lookup(name);

	/* if type is not defined yet */
	if(l->st_type != UNDEF) {
		l->st_type = SUBGRAPH_TYPE;

		l->parameters = parameter;

		return 0; // success
	}
	/* already declared */
	else {
		fprintf(stderr, "Subgraph %s already declared\n", name);
		exit(1);
	}
}

int subgraph_param_check(char *name, Param *parameter) {
	list_t *l = lookup(name);

	return 0;
}

// Functions for the Revisit Queue

void add_to_queue(char *name) {
	revisit_queue *q;

	/* if queue is empty */
	if(queue == NULL) {
		/* set up entry */
		q = (revisit_queue*) malloc(sizeof(revisit_queue));
		q->st_name = name;
		q->next = NULL;
		printf("Inserted %s into revisit queue\n", name);
		queue = q;
	}
	/* queue is not empty */
	else {
		q = queue;
		while(q->next != NULL) q = q-> next;

		/* add the element to the end of the queue */
		q->next = (revisit_queue*) malloc(sizeof(revisit_queue));
		q->next->st_name = name;
		q->next->next = NULL;
		printf("Inserted %s into revisit queue\n", name);
	}
}



/* when an entry is revisited it is also removed from the queue */
int revisit(char *name) {
	revisit_queue *q;

	/* special case in which the element is the first entry */
	if(strcmp(queue->st_name, name) == 0) {
		/* Might have to implement a parameter check? */

		queue = queue->next;
		printf("Deleted %s from revisit queue\n", name);
		return 0; //success
	}

	/* search for entry that points to element */
	q = queue;
	while(strcmp(q->next->st_name, name) != 0) q = q->next;

	/* check if entry was not found */
	if(q == NULL) {
		return 1; // entry was not found
	}

	/* once again, might have to implement some sort of paramter check */

	/* remove visited element by making previous entry point at next->next entry*/
	q->next = q->next->next;
	printf("Deleted %s from revisit queue\n", name);
	return 0; //success
}



void revisit_dump(FILE *of) {
	revisit_queue *q;
	q = queue;

	fprintf(of,"------------\n");
	fprintf(of,"Identifier\n");
	fprintf(of,"------------\n");
	while(q != NULL) {
		fprintf(of, "%-13s\n", q->st_name);
		fprintf(of, "\n");
		q = q->next;
	}
}

