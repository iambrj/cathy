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
