%{
#include <stdio.h>
#include <string.h>
#include "../dom/node.h"

extern int yylex(void);
extern int yyparse(void);
 
doc* document;

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
  struct snode* dn;
 }

%token START_EL END_EL SLASH
%token <string> WORD TEXT CDATA_TOK 
%type <string> value end_tag
%type <dn> attr node prop inner attrs declaration start_tag
%start document

%%
document: node             {document = new_document(NULL); set_doc_root(document, $1);}
	| declaration node {document = new_document($1); set_doc_root(document, $2);}
	;

declaration: START_EL '?' WORD attrs '?' END_EL {
						  if(strcmp($3, "xml") != 0){
						    yyerror("Declaration does not begin with xml.\n");
						    exit(-1);
						  }
						  $$ = $4;
						  set_name($$, $3);
						}
	;


node:	start_tag inner end_tag	 {				 
				  if(strcmp(get_name($1),$3) != 0){
				    yyerror("Start tag does not match end tag.\n");
				    exit(-1);
                                  }
				  append_children($1, $2->children);
				  destroy_dom_node($2);
				  $$ = $1;
				  }
        | START_EL WORD attrs SLASH END_EL { $$ = $3; set_name($$, $2);}
	;

inner:			{$$ = new_element_node("~dummy~");}
        | inner prop	{ $$ = $1;
			  append_child($$, $2);
			}
	;

prop:	CDATA_TOK  {$$ = new_cdata($1);}
	| TEXT {$$ = new_text_node($1);}
	| node {$$ = $1;}
	;

start_tag: START_EL WORD attrs END_EL {
					$$ = $3;
				        set_name($$, $2);
				      }
	;

end_tag: START_EL SLASH WORD END_EL {$$ = $3;}
	;

attrs: 		     {$$ = new_element_node("~dummy~");}
	| attrs attr {
		      $$ = $1;
		      add_attribute($$, $2);
		     }
	;

attr:	WORD '=' value {$$ = new_attribute($1, $3); }
	;

value:	'"' TEXT '"' {$$ = $2;}
        | '"' '"' {$$ = "";}
	;
