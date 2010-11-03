#include <stdlib.h>
#include <string.h>
#include "../include/query_parser.h"
#include "../include/macros.h"
#include "../include/y.tab.h"

extern int yyparse();

selector* new_selector(char* id){
  selector* r = alloc(selector, 1);
  r->id = (id == NULL)? NULL : strdup(id);
  r->filters = NULL;
  r->attrs = NULL;
  return r;
}

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

void destroy_selector(selector* s);

void destroy_filter_selector(filter_selector* fs){
  switch(fs->op){
  case NOT_FILTER:
    {
      int i;
      for(i = 0; i < fs->value.selector->count; i++)
	destroy_selector((selector*) get_element_at(fs->value.selector, i));
      destroy_generic_list(fs->value.selector);
      break;
    }
  case NTH_CHILD_FILTER:
  case NTH_LAST_CHILD_FILTER:
    free(fs->value.s);
  }
  free(fs);
}

void destroy_attr_selector(attr_selector* as){
  free(as->name);
  free(as->value);
  free(as);
}

void destroy_selector(selector* s){
  free(s->id);

  int i;
  for(i = 0; i < s->attrs->count; i++)
    destroy_attr_selector((attr_selector*) get_element_at(s->attrs, i));
  destroy_generic_list(s->attrs);

  for(i = 0; i < s->filters->count; i++)
    destroy_filter_selector((filter_selector*) get_element_at(s->filters, i));
  destroy_generic_list(s->filters);
  
  free(s);
}
