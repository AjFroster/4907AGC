/* fb2-4 text concordance */
%option noyywrap nodefault yylineno case-insensitive

/* the symbol table */
%{
struct symbol{
	char *name;  /* a word */
	struct ref *reflist;
};

struct ref{
	struct ref *next;
	char *filename;    
	int flags;    
	int lineno; 
};

/* simple symtab of fixed size */
#define NHASH 9997  
struct symbol symtab[NHASH];
struct symbol *lookup(char*);  
void addref(int, char*, char*,int);

char *curfilename; /* name of current input file */
%}
%%
