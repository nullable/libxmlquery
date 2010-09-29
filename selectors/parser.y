%{
#include <stdio.h>
#include <string.h>
#include "../dom/node.h"

extern int yylex(void);
extern int yyparse(void);
 

void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 
  
%}

%union{
  char * string;
  int digits;
 }

%token ALL SPACE EQUAL_OP WSSV_OP STARTSW_OP ENDSW_OP CONTAINS_OP DSV_OP NOTEQUAL_OP EVEN ODD
%token NTH_CHILD_FILTER NTH_LAST_CHILD_FILTER FIRST_CHILD_FILTER LAST_CHILD_FILTER ONLY_CHILD_FILTER EMPTY_FILTER NOT_FILTER
%token <string> WORD TEXT
%token <digits> DIGITS 
%start selector_group

%%

selector_group: selector
	      | selector_group relation_operator selector
	      ;

selector: id attrsel pseudo_filter 							{ printf("[selector]"); }
	;

attrsel: 
       | attrsel '[' WORD attr_filter ']'						{ printf("[attrsel] "); }
       ;


id: 											{ printf("[*] ");}
  | ALL
  | WORD										{ printf("[id] "); } 
  ;

pseudo_filter:
	     | pseudo_filter ':' pseudo_op						{ printf("[pseudo-filter: SIMPLE_FILTER] "); }
	     | pseudo_filter ':' nth_pseudo_op '(' parameters ')'			{ printf("[pseudo-filter: NTH_FILTER] "); }
	     | pseudo_filter ':' NOT_FILTER '(' selector_group ')' 				{ printf("[pseudo-filter: NOT_FILTER] "); }
	     ;



nth_pseudo_op:
	     | NTH_CHILD_FILTER
	     | NTH_LAST_CHILD_FILTER
	     ;

pseudo_op:
	 | FIRST_CHILD_FILTER
	 | LAST_CHILD_FILTER
	 | ONLY_CHILD_FILTER
	 | EMPTY_FILTER
	 ;

parameters: step
	  | offset
	  | EVEN
	  | ODD
	  ;

step: DIGITS 'n'
    | DIGITS 'n' offset
    ;

offset: '+' DIGITS
      | '-' DIGITS
      | DIGITS
      ;

relation_operator: '>'
		 | '~'
		 | '+'
		 | ','
		 | SPACE
		 ;

attr_filter: 
      | operator '"' TEXT '"'
      ;

operator: EQUAL_OP
	| WSSV_OP
	| STARTSW_OP
	| ENDSW_OP
	| CONTAINS_OP
	| DSV_OP
	| NOTEQUAL_OP
	;