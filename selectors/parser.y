%{
#include <stdio.h>
#include <string.h>
#include "../dom/node.h"
#include "query_parser.h"

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
  struct attr_selector_s* attrselector;
  struct step_s s;
  int token;
  struct filter_array_s fa;
 }

%token ALL SPACE EQUAL_OP WSSV_OP STARTSW_OP ENDSW_OP CONTAINS_OP DSV_OP NOTEQUAL_OP EVEN ODD
%token NTH_CHILD_FILTER NTH_LAST_CHILD_FILTER FIRST_CHILD_FILTER LAST_CHILD_FILTER ONLY_CHILD_FILTER EMPTY_FILTER NOT_FILTER
%token <string> WORD TEXT
%token <digits> DIGITS 

%type <attrselector> attr_filter;
%type <s> step parameters;
%type <digits> offset;
%type <token> operator pseudo_op nth_pseudo_op;
%type <fa> pseudo_filter;
%type <string> id;
%start selector_group

%%

selector_group: selector                                        { $$ = new_queue(16); enqueue_with_type($$, $1, 0); }
	          | selector_group relation_operator selector       { $$ = $1; enqueue_with_type($$, $2, 1); enqueue($$, $3, 0); }
	          ;

selector: id attrsel pseudo_filter 							    { $$ = new_selector($1); $$->attrs = $2; $$->filters = $3; }
	    ;

attrsels:                                                       { $$ = new_stack(4); }
        | attrsels '[' attrsel ']'					        	{ $$ = $1; push_stack($$, $3); }
        ;

attrsel: WORD attr_filter                                       { if($2 == NULL) { $$ = new_attr_name_selector($1); }
                                                                  else { $$ = $2; $$->name = $1; }
                                                                }
       ;

id: 					                						{ $$ = NULL; }
  | ALL									                		{ $$ = NULL; }
  | WORD									                	{ $$ = $1; printf("[id %s]\n", $1); } 
  ;

pseudo_filters:                                                 { $$ = new_stack(4); }
              | pseudo_filters ':' pseudo_filter                { push_stack($$, $3); }
              ;

pseudo_filter: pseudo_op						                { $$ = new_filter($1); printf("[pseudo-filter SIMPLE_FILTER]\n"); }
	         | nth_pseudo_op '(' parameters ')'	                { $$ = new_filter($1); $$->value.step = $5; printf("[pseudo-filter NTH_FILTER]\n"); }
	         | NOT_FILTER '(' selector_group ')' 	            { $$ = new_filter(NOT_FILTER); $$->value.s = $3; printf("[pseudo-filter NOT_FILTER]\n"); }
	         ;

nth_pseudo_op: NTH_CHILD_FILTER								    { $$ = NTH_CHILD_FILTER; }
	     | NTH_LAST_CHILD_FILTER							    { $$ = NTH_LAST_CHILD_FILTER; }
	     ;

pseudo_op: FIRST_CHILD_FILTER							    	{ $$ = FIRST_CHILD_FILTER; }
	 | LAST_CHILD_FILTER								        { $$ = LAST_CHILD_FILTER; }
	 | ONLY_CHILD_FILTER								        { $$ = ONLY_CHILD_FILTER; }
	 | EMPTY_FILTER									            { $$ = EMPTY_FILTER; }
	 ;

parameters: step									            { $$ = $1; }
	  | offset									                { $$.multiplier = 0; $$.offset = $1;}
	  | EVEN								                	{ $$.multiplier = 2; $$.offset = 0; }
	  | ODD										                { $$.multiplier = 2; $$.offset = 1; }
	  ;

step: 'n'										                { $$.multiplier = 1; $$.offset = 0; } 
    | DIGITS 'n'									            { $$.multiplier = $1; $$.offset = 0; }
    | 'n' offset								            	{ $$.multiplier = 1; $$.offset = $2; }
    | DIGITS 'n' offset									        { $$.multiplier = $1; $$.offset = $3; }
    ;

offset: '+' DIGITS									            { $$ = $2; }
      | '-' DIGITS									            { $$ = -$2; }
      | DIGITS										            { $$ = $1; }
      ;

relation_operator: '>'			        						//{ printf("[op children]\n"); }
		 | '~'							                		//{ printf("[op siblings]\n"); }
		 | '+'              									//{ printf("[op adj siblings]\n"); }
		 | ','				                					//{ printf("[op save]\n"); }
		 | SPACE			                					//{ printf("[op descendents]\n"); }
		 ;

attr_filter: 				            						{ $$ = NULL; }
      | operator '"' TEXT '"'           						{ $$ = new_attr_value_selector(NULL, $1, $3); }
      ;

operator: EQUAL_OP          									{ $$ = EQUAL_OP; }
	| WSSV_OP				            		    			{ $$ = WSSV_OP; }
	| STARTSW_OP			            						{ $$ = STARTSW_OP; }
	| ENDSW_OP				            		    			{ $$ = ENDSW_OP; }
	| CONTAINS_OP			            						{ $$ = CONTAINS_OP; }
	| DSV_OP				            		    			{ $$ = DSV_OP; }
	| NOTEQUAL_OP			            						{ $$ = NOTEQUAL_OP; }
	;
	
