%{
#include <stdio.h>
#include <string.h>
#include "../include/node.h"
#include "../include/stack.h"
#include "../include/query_parser.h"
#include "../include/macros.h"

#define LXQ_RELATION_TYPE 0
#define LXQ_SELECTOR_TYPE 1

extern int yylex(void);
extern int yyparse(void);
extern int yylineno;
extern char* yytext;
extern FILE* yyin;

doc* lxq_document;

char* lxq_parser_dot_query_operator = "class";
char* lxq_parser_pound_query_operator = "type";
list* lxq_selected_elements;

void yyerror(const char *str)
{
  fprintf(stderr,"error:%d: %s at '%s'\n", yylineno, str, yytext);
}

int yywrap()
{
        return 1;
}

void parse_file(char* filename){
  char* address;
  FILE* holder;
  struct yy_buffer_state* bs;

  holder = yyin = fopen(filename, "r");
  if(yyin == NULL){
    log(F, "Unnable to open file %s for reading\n", filename);
    exit(-1);
  }
  yylineno = 1;
  yyparse();
  yylex_destroy();
  fclose(holder);
}

void parse_string(const char* str){
  int len = strlen(str);
  char* internal_cpy = alloc(char, len + 2);
  struct yy_buffer_state* bs;

  memcpy(internal_cpy, str, len);

  internal_cpy[len] = '\0';
  internal_cpy[len + 1] = '\0';

  bs = (struct yy_buffer_state*) yy_scan_buffer(internal_cpy, len + 2);
  if(bs == NULL){
    log(F, "flex could not allocate a new buffer to parse the string.\n");
    exit(-1);
  }
  yylineno = 1;
  yyparse();
  yylex_destroy();
  free(internal_cpy);
}

%}

%union{
  char * string;
  struct snode* dn;
  int digits;
  struct attr_selector_s* attrselector;
  struct step_s* s;
  int token;
  struct filter_selector_s* fa;
  struct generic_list_s* q;
  struct selector_s* sel;
 }

%token START_EL END_EL SLASH
%token <string> WORD TEXT CDATA_TOK REGEX
%token ALL SPACE NO_OP EQUAL_OP WSSV_OP STARTSW_OP ENDSW_OP CONTAINS_OP REGEX_OP REGEXI_OP DSV_OP NOTEQUAL_OP EVEN ODD
%token NTH_CHILD_FILTER NTH_LAST_CHILD_FILTER FIRST_CHILD_FILTER LAST_CHILD_FILTER ONLY_CHILD_FILTER EMPTY_FILTER NOT_FILTER
%type <string> value
%type <dn> attr node prop inner attrs declaration start_tag end_tag namespace
%token <digits> DIGITS

%type <attrselector> attr_filter attrsel;
%type <s> step parameters;
%type <digits> offset;
%type <token> operator pseudo_op nth_pseudo_op relation_operator rmodifier;
%type <fa> pseudo_filter;
%type <string> id;
%type <q> selector_group pseudo_filters attrsels regex
%type <sel> selector

%start choose

%%

choose: '@' selector_group
      | document
      ;

document: node                                              {lxq_document = new_document(NULL); set_doc_root(lxq_document, $1);}
        | declaration node                                  {lxq_document = new_document($1); set_doc_root(lxq_document, $2);}
        ;

namespace: WORD                                             { $$ = new_element_node($1);}
         | WORD ':' WORD                                    { $$ = new_element_node($3); 
                                                              char* old = set_namespace($$, $1);
							      if(old)
								free(old);
	                                                    }
         ;

declaration: START_EL '?' namespace attrs '?' END_EL        {
                                                              if(strcmp(get_name($3), "xml") != 0){
                                                                yyerror("Declaration does not begin with xml");
                                                                exit(-1);
                                                              }
                                                              $$ = $4;
                                                              char* old = set_name($$, get_name($3));
							      if(old)
								free(old);
                                                              old = set_namespace($$, get_namespace($3));
							      if(old)
								free(old);
                                                              destroy_dom_node($3);
                                                            }
           ;

node: start_tag inner end_tag                               {
                                                              if(strcmp(get_name($1),get_name($3)) != 0)
                                                              {
								char error_line[1024] = {0};
								sprintf(error_line, "Start tag '%s' does not match end tag '%s' ", get_name($1), get_name($3));
                                                                yyerror(error_line);
                                                                exit(1);
                                                              }
                                                              append_children($1, $2->children);
							      if($2->namespace != NULL)
								free($2->namespace);
                                                              free($2->name);
                                                              destroy_generic_list($2->children);
                                                              free($2);
                                                              $$ = $1;
							      destroy_dom_node($3);
                                                            }
    | START_EL namespace attrs SLASH END_EL                 { $$ = $3;
                                                              char* old = set_name($$, get_name($2));
							      if(old)
								free(old);							    
                                                              old = set_namespace($$, get_namespace($2));
							      if(old)
								free(old);
                                                              destroy_dom_node($2);
                                                            }
    ;

inner:                                                      { $$ = new_element_node("");}
     | inner prop                                           { $$ = $1;
                                                              append_child($$, $2);
                                                            }
     ;

prop: CDATA_TOK                                             {$$ = new_cdata($1);}
    | TEXT                                                  {$$ = new_text_node($1); }
    | node                                                  {$$ = $1;}
    ;


start_tag: START_EL namespace attrs END_EL                  { $$ = $3;
                                                              char* old = set_name($$, get_name($2));
							      if(old)
								free(old);
                                                              old = set_namespace($$, get_namespace($2));
							      if(old)
								free(old);
                                                              destroy_dom_node($2);
                                                            }
         ;

end_tag: START_EL SLASH namespace END_EL                    { $$ = $3;}
       ;

attrs:                                                      { $$ = new_element_node(""); }
     | attrs attr                                           {
                                                              $$ = $1;
                                                              add_attribute($$, $2);
                                                            }
     ;

attr:  namespace '=' value                                  {$$ = new_attribute(get_name($1), $3); 
                                                             char* old = set_namespace($$, get_namespace($1)); 
							     if(old)
							       free(old);
							     destroy_dom_node($1); }
    ;


value: '"' TEXT '"'                                         {$$ = $2;}
     | '"' '"'                                              {$$ = "";}
     ;


selector_group: selector                                    { lxq_selected_elements = $$ = new_queue(16); enqueue_with_type($$, $1, LXQ_SELECTOR_TYPE); }
              | selector_group relation_operator selector   { int* a = alloc(int, 1);
                                                              *a = $2;
                                                              $$ = $1;
                                                              enqueue_with_type($$, a, LXQ_RELATION_TYPE);
                                                              enqueue_with_type($$, $3, LXQ_SELECTOR_TYPE);
                                                            }
              ;

selector: id attrsels pseudo_filters                        { $$ = new_selector($1); $$->attrs = $2; $$->filters = $3; }
        ;

attrsels:                                                   { $$ = new_stack(4); }
        | attrsels '[' attrsel ']'                          { $$ = $1; push_stack($$, $3); }
        | attrsels '.' WORD                                 { $$ = $1; push_stack($$, new_attr_value_selector(lxq_parser_dot_query_operator, EQUAL_OP, $3)); }
        | attrsels '#' WORD                                 { $$ = $1; push_stack($$, new_attr_value_selector(lxq_parser_pound_query_operator, EQUAL_OP, $3)); }
        ;

attrsel: WORD attr_filter                                   { $$ = $2; $$->name = strdup($1); }
       ;

id:                                                         { $$ = NULL; }
  | ALL                                                     { $$ = NULL; }
  | WORD                                                    { $$ = $1;}
  ;

pseudo_filters:                                             { $$ = new_stack(4); }
              | pseudo_filters ':' pseudo_filter            { push_stack($$, $3); }
              ;

pseudo_filter: pseudo_op                                    { $$ = new_filter($1); }
             | nth_pseudo_op '(' parameters ')'             { $$ = new_filter($1); $$->value.s = $3; }
             | NOT_FILTER '(' selector_group ')'            { $$ = new_filter(NOT_FILTER); $$->value.selector = $3; }
             ;

nth_pseudo_op: NTH_CHILD_FILTER                             { $$ = NTH_CHILD_FILTER; }
             | NTH_LAST_CHILD_FILTER                        { $$ = NTH_LAST_CHILD_FILTER; }
             ;

pseudo_op: FIRST_CHILD_FILTER                               { $$ = FIRST_CHILD_FILTER; }
         | LAST_CHILD_FILTER                                { $$ = LAST_CHILD_FILTER; }
         | ONLY_CHILD_FILTER                                { $$ = ONLY_CHILD_FILTER; }
         | EMPTY_FILTER                                     { $$ = EMPTY_FILTER; }
         ;

parameters: step                                            { $$ = $1; }
          | offset                                          { $$ = alloc(struct step_s, 1); $$->multiplier = 0; $$->offset = $1;}
          | EVEN                                            { $$ = alloc(struct step_s, 1); $$->multiplier = 2; $$->offset = 0; }
          | ODD                                             { $$ = alloc(struct step_s, 1); $$->multiplier = 2; $$->offset = 1; }
          ;

step: 'n'                                                   { $$ = alloc(struct step_s, 1); $$->multiplier = 1; $$->offset = 0; }
    | DIGITS 'n'                                            { $$ = alloc(struct step_s, 1); $$->multiplier = $1; $$->offset = 0; }
    | 'n' offset                                            { $$ = alloc(struct step_s, 1); $$->multiplier = 1; $$->offset = $2; }
    | DIGITS 'n' offset                                     { $$ = alloc(struct step_s, 1); $$->multiplier = $1; $$->offset = $3; }
    ;

offset: '+' DIGITS                                          { $$ = $2; }
      | '-' DIGITS                                          { $$ = -$2; }
      | DIGITS                                              { $$ = $1; }
      ;

relation_operator: '>'                                      { $$ = '>'; }
                 | '~'                                      { $$ = '~'; }
                 | '+'                                      { $$ = '+'; }
                 | ','                                      { $$ = ','; }
                 | SPACE                                    { $$ = SPACE; }
                 ;

attr_filter:                                                { $$ = new_attr_value_selector(NULL, NO_OP, NULL); }
           | operator '"' TEXT '"'                          { $$ = new_attr_value_selector(NULL, $1, $3); }
           | operator '\'' TEXT '\''                        { $$ = new_attr_value_selector(NULL, $1, $3); }
           | EQUAL_OP '/' regex '/' rmodifier               {
                                                                char* text = (char*)pop_stack($3);
                                                                while($3->count > 0){
                                                                    char* r = (char*)pop_stack($3);
                                                                    text = (char*)realloc(text, strlen(text) + strlen(r) + 1);
                                                                    strcat(text, r);
                                                                }
                                                                $$ = new_attr_value_selector(NULL, $5, text);
                                                                destroy_generic_list($3);
                                                            }
           ;

regex: REGEX                                                { $$ = new_stack(4); push_stack($$, $1); }
     | regex REGEX                                          { $$ = $1; push_stack($$, $2); }
     ;

rmodifier:                                                  { $$ = REGEX_OP; }
         | 'i'                                              { $$ = REGEXI_OP; }
         ;

operator: EQUAL_OP                                          { $$ = EQUAL_OP; }
        | WSSV_OP                                           { $$ = WSSV_OP; }
        | STARTSW_OP                                        { $$ = STARTSW_OP; }
        | ENDSW_OP                                          { $$ = ENDSW_OP; }
        | CONTAINS_OP                                       { $$ = CONTAINS_OP; }
        | DSV_OP                                            { $$ = DSV_OP; }
        | NOTEQUAL_OP                                       { $$ = NOTEQUAL_OP; }
        ;

