/* max size of hash table */
#define SIZE 211

/* max size of token id */
#define MAXTOKENLEN 40

/* token types */
#define UNDEF 0

typedef struct Param{
	char par_name[MAXTOKENLEN];
	int ival; double fval; char *st_sval;
} Param;

/* linked list of references for each identifier */
typedef struct RefList {
	int lineno;
	struct RefList *next;
	int type;
} RefList;

// struct representing list node
typedef struct list_t {
	char st_name[MAXTOKENLEN];
	int st_size;
	RefList *lines;
	int st_ival; double st_fval; char *st_sval;
	// type
	int st_type;
	// function parameters
	Param *parameters;
	int num_of_pars;
	// pointer to next item in list
	struct list_t *next;
} list_t;

/* hash table */
static list_t **hash_table;


// function declarations
void init_hash_table();
unsigned int hash(char *key);
void insert(char *name, int len, int type, int lineno);
list_t *lookup(char *name);
<<<<<<< HEAD
void symtab_dump(FILE *of);
=======
<<<<<<< HEAD
void symtab_dump(FILE *of);
=======
void symtab_dump(FILE *of);
>>>>>>> 10550545bb746e667bf7a40ab38cc61e28b0b35f
>>>>>>> 4ee175b10d8d0572e36b9d8b0629aa027697b1c6
