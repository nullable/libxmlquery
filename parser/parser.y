%{
#include <stdio.h>
#include <string.h>
#include "../dom/node.h"
#include "../data_structures/stack.h"
#include "../selectors/query_parser.h"
#include "../dom/macros.h"

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
%token ALL SPACE EQUAL_OP WSSV_OP STARTSW_OP ENDSW_OP CONTAINS_OP REGEX_OP REGEXI_OP DSV_OP NOTEQUAL_OP EVEN ODD
%token NTH_CHILD_FILTER NTH_LAST_CHILD_FILTER FIRST_CHILD_FILTER LAST_CHILD_FILTER ONLY_CHILD_FILTER EMPTY_FILTER NOT_FILTER
%type <string> value end_tag
%type <dn> attr node prop inner attrs declaration start_tag
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

document: node                                              {document = new_document(NULL); set_doc_root(document, $1);}
  | declaration node                                        {document = new_document($1); set_doc_root(document, $2);}
  ;

declaration: START_EL '?' WORD attrs '?' END_EL             {
                                                              if(strcmp($3, "xml") != 0){
                                                              yyerror("Declaration does not begin with xml.\n");
                                                              exit(-1);
                                                              }
                                                              $$ = $4;
                                                              set_name($$, $3);
                                                            }
           ;



node: start_tag inner end_tag                               {
                                                              if(strcmp(get_name($1),$3) != 0)
                                                              {
                                                                yyerror("Start tag does not match end tag.\n");
                                                                exit(-1);
                                                              }
                                                              append_children($1, $2->children);
                                                              free($2->name);
                                                              destroy_generic_list($2->children);
                                                              free($2);
                                                              $$ = $1;
                                                            }
    | START_EL WORD attrs SLASH END_EL                      { $$ = $3; set_name($$, $2);}
    ;

inner:                                                      { $$ = new_element_node("~dummy~");}
     | inner prop                                           { $$ = $1;
                                                              append_child($$, $2);
                                                            }
     ;

prop: CDATA_TOK                                             {$$ = new_cdata($1);}
    | TEXT                                                  {$$ = new_text_node($1);}
    | node                                                  {$$ = $1;}
    ;


start_tag: START_EL WORD attrs END_EL                       { $$ = $3;
                                                              set_name($$, $2);
                                                            }
         ;

end_tag: START_EL SLASH WORD END_EL                         { $$ = $3; }
       ;

attrs:                                                      { $$ = new_element_node("~dummy~"); }
     | attrs attr                                           {
                                                              $$ = $1;
                                                              add_attribute($$, $2);
                                                            }
     ;

attr:  WORD '=' value                                       {$$ = new_attribute($1, $3);}
    ;


value: '"' TEXT '"'                                         {$$ = $2;}
     | '"' '"'                                              {$$ = "";}
     ;

selector_group: selector                                    { $$ = new_queue(16); enqueue_with_type($$, $1, 0); }
              | selector_group relation_operator selector   { int* a = alloc(int, 1);
                                                              *a = $2;
                                                              $$ = $1;
                                                              enqueue_with_type($$, a, 1);
                                                              enqueue_with_type($$, $3, 0);
                                                            }
              ;

selector: id attrsels pseudo_filters                        { $$ = new_selector($1); $$->attrs = $2; $$->filters = $3; }
        ;

attrsels:                                                   { $$ = new_stack(4); }
        | attrsels '[' attrsel ']'                          { $$ = $1; push_stack($$, $3); }
        ;

attrsel: WORD attr_filter                                   { if($2 == NULL) {
                                                                $$ = new_attr_name_selector($1);
                                                              }
                                                              else {
                                                                $$ = $2;
                                                                $$->name = $1;
                                                              }
                                                            }
       ;

id:                                                         { $$ = NULL; }
  | ALL                                                     { $$ = NULL; }
  | WORD                                                    { $$ = $1; }
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

attr_filter:                                                { $$ = NULL; }
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
          | 'i'                                             { $$ = REGEXI_OP; }
          ;

operator: EQUAL_OP                                          { $$ = EQUAL_OP; }
        | WSSV_OP                                           { $$ = WSSV_OP; }
        | STARTSW_OP                                        { $$ = STARTSW_OP; }
        | ENDSW_OP                                          { $$ = ENDSW_OP; }
        | CONTAINS_OP                                       { $$ = CONTAINS_OP; }
        | DSV_OP                                            { $$ = DSV_OP; }
        | NOTEQUAL_OP                                       { $$ = NOTEQUAL_OP; }
        ;

