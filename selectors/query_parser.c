#include "query_parser.h"
#include "../dom/macros.h"

extern int yyparse();

selector* new_selector(char* id){
  selector* r = alloc(selector, 1);
  r->id = id;
  
  r->attr_count = 0;
  r->attrs = alloc(attr_selector*, 1);
  r->attr_capacity = 1;
  
  r->filters = new_filter_array();
  return r;
}

attr_selector* new_attr_name_selector(char* name){
  attr_selector* r = alloc(attr_selector, 1);
  r->name = name;
  return r;
}

attr_selector* new_attr_value_selector(char* name, int op, char* value){
  attr_selector* r = new_attr_name_selector(name);
  r->op = op;
  r->value = value;
  return r;
}

filter_selector* new_filter(int filter){
  filter_selector* r = alloc(filter_selector, 1);
  r->op = filter;
  return r;
}


// void add_attr_selector(selector* s, attr_selector* a){
//   if(s->attr_count == s->attr_capacity){
//     s->attrs = realloc(s->attrs, sizeof(attr_selector*) * s->attr_capacity*2);
//     s->attr_capacity *= 2;
//   }
//   s->attrs[++(s->attr_count)] = a;
// }

// void add_filter_selector(filter_array* fa, filter_selector* f){
//   if(fa->count == fa->capacity){
//     fa->array = realloc(fa->array, sizeof(filter_selector*) * fa->capacity*2);
//     fa->capacity *= 2;
//   }
//   fa->array[++(fa->count)] = f;
// }

int main(){
  yyparse();
  return 0;
}
