#include <stdlib.h>
#include <string.h>
#include "query_parser.h"
#include "../dom/macros.h"

extern int yyparse();

selector* new_selector(char* id){
  selector* r = alloc(selector, 1);
  r->id = (id == NULL)? NULL : strdup(id);
  r->filters = NULL;
  r->attrs = NULL;
  return r;
}

/*
attr_selector* new_attr_name_selector(char* name){
  attr_selector* r = alloc(attr_selector, 1);
  r->name = name;
  return r;
}
*/

attr_selector* new_attr_value_selector(char* name, int op, char* value){
  attr_selector* r = alloc(attr_selector, 1);
  r->name = (name == NULL)? NULL : strdup(name);
  r->op = op;
  r->value = (value == NULL)? NULL : strdup(value);
  return r;
}

filter_selector* new_filter(int filter){
  filter_selector* r = alloc(filter_selector, 1);
  r->op = filter;
  return r;
}

