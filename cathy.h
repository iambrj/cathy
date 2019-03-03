/* symbol table */
struct symbol 
{
	char *name; /* variable identifiers */
	double value;
	struct ast *func; /* function identifiers */
	struct symlist *syms; /* function arguments */
}
/* fixed size symtab */
#define NHASH 9999
struct symbol symtab[NHASH];
struct symbol *lookup(char*);
struct symlist
{
	struct symbol *sym;
	struct symlist *next;
};


/* nodetypes
 * + - * / |
 * 0 - 7 comparision ops, bit coded 04 equal, 02 less, 01 greater
 * M unary minus
 * L statement list
 * I IF statement
 * W while statement
 * N symbol ref
 * = assignment
 * S list of symbols
 * F built in function call
 * C user function call
 */
struct ast 
{
	int nodetype;
	struct ast *l;
	struct ast *r;
};
struct numval 
{
	int nodetype;
	double number;
};

enum bifs /* built-in functions */
{
	B_sqrt = 1,
	B_exp,
	B_log,
	B_print
};

struct fncall /* built-in functions */
{
	int nodetype; /* type F */
	struct ast *l;
	enum bifs functype;
};

struct ufncall /* user functions */
{
	int nodetype; /* type C */
	struct ast *l; /* args */
	struct symbol *s;
};

struct flow
{
	int nodetype; /* type I or W */
	struct ast *cond; /* condition */
	struct ast *tl; /* then list */
	struct ast *el; /* else list */
};

struct symref
{
	int nodetype; /* type N */
	struct symbol *s;
};

struct symasgn
{
	int nodetype; /* type = */
	struct symbol *s;
	struct ast *v;
};

/* function declarations */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newcall(struct symbol *s, struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);
void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts);
double eval(struct ast *);
void treefree(struct ast *);

/* lexer interface */
extern int yylineno;
void yyerror(char* s, ...);
