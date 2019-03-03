%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "cathy.h"
%}

%union {
	struct ast *a;
	double d;
	struct symbol *s;
	struct symlist *sl;
	int fn;
};

/* declare tokens */
%token <d> NUMBER
%token <s> NAME
%token <fn> FUNC
%token EOL
%token IF THEN ELSE WHILE DO LET

%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/'
%nonassoc '|' UMINUS

%type <a> exp stmt list explist
%type <sl> symlist
%start cathy

%%

cathy: /* epsilon */
	| cathy stmt EOL { 
					if(debug) dumpast($2, 0);
					printf(" = %4.4g\n> ", eval($2)); 
					treefree($2);
					}
	| cathy LET NAME '(' symlist ')' '=' list EOL {
													dodef($3, $5, $8);
													printf("Defined %s\n> ", $3->name); }
	| cathy error EOL {yyerrok, printf("> "); }
	;

stmt: IF exp THEN list { $$ = newflow('I', $2, $4, NULL); }
	| IF exp THEN list ELSE list { $$ = newflow('I', $2, $4, $6); }
	| WHILE exp DO list { $$ = newflow('W', $2, $4, NULL); }
	| exp
	;

symlist: NAME { $$ = newsymlist($1, NULL); }
	   | NAME ',' symlist { $$ = newsymlist($1, $3); }
	   ;
list: /* epsilon */ { $$ = NULL; }
	| stmt ';' list {
					if ($3 == NULL)
						$$ = $1;
					else
						$$ = newast('L', $1, $3);
					}
	;

exp: exp CMP exp			{ $$ = newmcp($2, $1, $3); }
   | exp '+' exp 			{ $$ = newast('+', $1, $3); }
   | exp '-' exp 			{ $$ = newast('-', $1, $3); }
   | exp '*' exp 			{ $$ = newast('*', $1, $3); }
   | exp '/' exp 			{ $$ = newast('/', $1, $3); }
   | '|' exp 				{ $$ = newast('|', $2, NULL); }
   | '(' exp ')' 			{ $$ = $2; }
   | '-' exp %prec UMINUS 	{ $$ =newast('M', $2, NULL); }
   | NUMBER 				{ $$ = newnum($1); }
   | FUNC '(' explist ')' 	{ $$ = newfunc($1, $3); }
   | NAME					{ $$ = newref($1); }
   | NAME '=' exp 			{ $$ = newasgn($1, $3); }
   | NAME '(' explist ')' 	{ $$ = newcall($1, $3); }
   ;

explist: exp /* nothing */
	   | exp ',' explist { $$ = newast('L', $1, $3); }
%%
