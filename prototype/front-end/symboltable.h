/* max size of hash table */
#define SIZE 211

/* max size of token id */
#define MAXTOKENLEN 40

/*token type*/
#define UNDEF 0
#define INT_TYPE 1
#define SUBGRAPH_TYPE 2

/*operator types*/
#define NONE 0
#define ARITHM_OP 1
#define REL_OP 2


typedef struct Param{
	char par_name[MAXTOKENLEN];
	int ival;
} Param;

/* linked list of references for each identifier */
typedef struct RefList {
	int lineno;
	struct RefList *next;
} RefList;

// struct representing list node
typedef struct list_t {
	char st_name[MAXTOKENLEN];
	int st_size;
	int scope;
	RefList *lines;
	int st_ival;
	// type
	int st_type;
	// function parameters
	Param *parameters;
	int num_of_pars;
	// pointer to next item in list
	struct list_t *next;
} list_t;

/* Queue of identifiers to revisit */ 
typedef struct revisit_queue {
	// identifier name
	char *st_name;

	//type of revisit
	//int result_type;

	struct revisit_queue *next;
}revisit_queue;

#define PARAM_CHECK 1 /* this is to check parameters of function call when functions get declared */

/* hash table */
static list_t **hash_table;
static revisit_queue *queue;


// symbol table function declarations
void init_hash_table();
unsigned int hash(char *key);
void insert(char *name, int len, int type, int lineno);
list_t *lookup(char *name);
void symtab_dump(FILE *of);

// scope management function declarations
void hide_scope();
void incr_scope();

// Subgraph Declaration
Param def_param(char *param_name);
int subgraph_declare(char *name, Param *parameter); 
int subgraph_param_check(char *name, Param *parameter);

// Revisit Queue Functions
void add_to_queue(char *name);
int revisit(char *name);
void revisit_dump(FILE *of); 
