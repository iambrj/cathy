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
