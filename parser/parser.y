%{
#include <stdio.h>
#include <string.h>

int yylex(void);
int yyparse(void);
 
void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 
  
int main()
{
        yyparse();
	return 0;
} 

%}

%token WORD START_EL END_EL SLASH TEXT CDATA_TOK
%start document

%%
document: node 
	| declaration node
	;

declaration: START_EL '?' WORD attrs '?' END_EL
	;


node:	start_tag inner end_tag	    
	;

inner:	inner prop
	| prop
	;

prop:	CDATA_TOK
	| TEXT
	| node
	;

start_tag: START_EL WORD attrs END_EL
	;

end_tag: START_EL SLASH WORD END_EL
	;

attrs: 
	| attrs attr
	;

attr:	WORD '=' value
	;

value:	'"' TEXT '"'
	;

