#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "cathy.h"

static unsigned symhash(char *sym)
{
	unsigned int hash = 0;
	unsigned c;

	while(c = *sym++) hash = hash*9 ^c;
	return hash;
}

struct ast *newast(int nodetype, struct ast *l, struct ast *r)
{
	struct ast *a = malloc(sizeof(struct ast));

	if(!a) 
	{
		yyerror("Out of space!");
		exit(0);
	}
	a->nodetype = nodetype;
	a->l = l;
	a->r = r;
	return a;
}

struct symbol *lookup(char *sym)
{
	struct symbol *sp = &symtab[symhash(sym) % NHASH];
	int scount = NHASH;

	while(--scount >= 0)
	{
		if(sp->name && !strcmp(sp->name, sym)) return sp;
		if(!sp->name)
		{
			sp->name = strdup(sym);
			sp->value = 0;
			sp->func = NULL;
			sp->syms = NULL;
			return sp;
		}
		if(++sp >= symtab + NHASH) sp = symtab;
	}
	yyerror("symbol table overflow\n");
	abort();
}

struct ast *newnum(double d)
{
	struct numval *a = malloc(sizeof(struct numval));

	if(!a)
	{
		yyerror("Out of space!");
		exit(0);
	}
	a->nodetype = 'K';
	a->number = d;
	return (struct ast *)a;
}

struct ast *newcmp(int cmptype, struct ast *l, struct ast *r)
{
	struct ast *a = malloc(sizeof(struct ast));

	if(!a)
	{
		yyerror("Out of space!");
		exit(0);
	}
	a->nodetype = '0' + cmptype;
	a->l = l;
	a->r = r;
	return a;
}

struct ast *newfunc(int functype, struct ast *l)
{
	struct fncall *a = malloc(sizeof(struct fncall));

	if(!a)
	{
		yyerror("Out of space!");
		exit(0);
	}
	a->nodetype = 'F';
	a->l = l;
	a->functype = functype;
	return (struct ast *)a;
}

struct ast *newcall(struct symbol *s, struct ast *l)
{
	struct ufncall *a = malloc(sizeof(struct ufncall));

	if(!a)
	{
		yyerror("Out of space!");
		exit(0);
	}
	a->nodetype = 'C';
	a->l = l;
	a->s = s;
	return (struct ast *)a;
}

struct ast *newref(struct symbol *s)
{
	struct symref *a = malloc(sizeof(struct symref));

	if(!a)
	{
		yyerror("Out of space!");
		exit(0);
	}
	a->nodetype = 'N';
	a->s = s;
	return (struct ast *)a;
}

struct ast *newasgn(struct symbol *s, struct ast *v)
{
	struct symasgn *a = malloc(sizeof(struct symasgn));

	if(!a)
	{
		yyerror("Out of space!");
		exit(0);
	}
	a->nodetype = '=';
	a->s = s;
	a->v = v;
	return (struct ast *)a;
}
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *el)
{
	struct flow *a = malloc(sizeof(struct flow));

	if(!a)
	{
		yyerror("Out of space!");
		exit(0);
	}
	a->nodetype = nodetype;
	a->cond = cond;
	a->tl = tl;
	a->el = el;
	return (struct ast *)a;
}

struct symlist *newsymlist(struct symbol *sym, struct symlist *next)
{
	struct symlist *sl = malloc(sizeof(struct symlist));

	if(!sl)
	{
		yyerror("Out of space");
	}
	sl->sym = sym;
	sl->next = next;
	return sl;
}

void symlistfree(struct symlist *sl)
{
	struct symlist *nsl;

	while(sl)
	{
		nsl = sl->next;
		free(sl);
		sl = nsl;
	}
}

void dodef(struct symbol *name, struct symlist *syms, struct ast *func)
{
	if(name->syms) symlistfree(name->syms);
	if(name->func) treefree(name->func);
	name->syms = syms;
	name->func = func;
}

double eval(struct ast *a) //runs a depth first search on ast *a and return its value
{
	double v;

	if(!a)
	{
		yyerror("Internal error, null eval");
		return 0.0;
	}
	switch(a->nodetype)
	{
		/* constant */
		case 'K': v = ((struct numval *) a)->number; break;

		/* name reference */
		case 'N': v = ((struct symref *) a)->s->value; break;

		/* assignment */
		case '=': v = ((struct symasgn *)a )->s->value = eval(((struct symasgn *)a)->v); break;

		/* expressions */
		case '+': v = eval(a->l) + eval(a->r); break;
		case '-': v = eval(a->l) - eval(a->r); break;
		case '*': v = eval(a->l) * eval(a->r); break;
		case '/': v = eval(a->l) / eval(a->r); break; 
		case '|': v = fabs(eval(a->l)); break;
		case 'M': v = -eval(a->l); break; //mod

		/* comparision */
		case '1': v = (eval(a->l) < eval(a->r)); break;
		case '2': v = (eval(a->l) > eval(a->r)); break;
		case '3': v = (eval(a->l) != eval(a->r)); break;
		case '4': v = (eval(a->l) == eval(a->r)); break;
		case '5': v = (eval(a->l) >= eval(a->r)); break;
		case '6': v = (eval(a->l) <= eval(a->r)); break;

		/* control flow */
		case 'I':
				  if(eval(((struct flow *)a)->cond) != 0)
				  {
					  if(((struct flow *)a)->tl)
					  {
						  v = eval(((struct flow *)a)->tl);
					  }
					  else
					  {
						  v = 0.0; /* default value */
					  }
				  }
				  else
				  {
					  if(((struct flow *)a)->el)
					  {
						  v = eval(((struct flow *)a)->el);
					  }
					  else
					  {
						  v = 0.0; /* default value */
					  }
				  }
				  break;

		case 'W':
				  v = 0.0;
				  if(((struct flow *)a)->tl)
				  {
					  while(eval(((struct flow *)a)->cond) != 0)
					  {
						  v = eval(((struct flow *)a)->tl);
					  }
				  }
				  break;

		case 'L': eval(a->l); v = eval(a->r); break;

		case 'F': v = callbuiltin((struct fncall *)a); break;

		case 'C': v = calluser((struct ufncall *)a); break;

		default: printf("Internal error: bad node %c\n", a->nodetype);
	}
	return v;
}

void yyerror(char *s, ...)
{
	va_list ap;
	va_start(ap, s);

	fprintf(stderr, "%d: error: ", yylineno);
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}

int main()
{
	printf("> ");
	return yyparse();
}
