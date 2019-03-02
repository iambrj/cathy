#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "cathy.h"

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

double eval(struct ast *a) //runs a depth first search on ast *a
{
	double v;
	switch(a->nodetype)
	{
		case 'K': v = ((struct numval *) a)->number; break; //constant
		case '+': v = eval(a->l) + eval(a->r); break;
		case '-': v = eval(a->l) - eval(a->r); break;
		case '*': v = eval(a->l) * eval(a->r); break;
		case '/': v = eval(a->l) / eval(a->r); break;
		case '|': v = eval(a->l); if(v < 0) v = -v; break;
		case 'M': v = -eval(a->l); break; //mod
		default: printf("internal error: bad node %c\n", a->nodetype);
	}
	return v;
}
void treefree(struct ast *a)
{
	switch(a->nodetype) 
	{
		/* two subtree cases */
		case '+':
		case '-':
		case '*':
		case '/':
			treefree(a->r);
		/* one subtree cases */
		case '|':
		case 'M':
			treefree(a->l);
		/* no subtree case */
		case 'K':
			free(a);
			break;

		default: printf("internal error: free bad node %c\n", a->nodetype);
	}
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
