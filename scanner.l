%{
	#include "parser.tab.h"
%}
%%
"+" { return ADD; }
"-" { return SUB; }
"*" { return MUL; }
"/" { return DIV; }
"|" { return ABS; }
"(" { return OP; }
")" { return CP; }
"//".* /* ignore comments */
[0-9]+ { yylval = atoi(yytext); return NUMBER; }
[0-9]+.[0-9]+ { yylval = atof(yytext); return NUMBER; }
\n { return EOL; }
[ \t] { }
. { printf("Mystery character %s\n", yytext); }
%%
