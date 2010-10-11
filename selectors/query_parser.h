#ifndef __QUERY_PARSER_H__
#define __QUERY_PARSER_H__

#include "../data_structures/stack.h"

struct selector_s;

typedef struct attr_selector_s{
  char* name;
  int op;
  char* value;
} attr_selector;

typedef struct step_s{ int multiplier; int offset; } step;

typedef struct filter_selector_s{
  int op;
  union {
    step* s;
    struct generic_list_s* selector;
  } value;
} filter_selector;

typedef struct selector_s{
  char* id;
  stack* attrs;
  stack* filters;
} selector;

selector* new_selector(char* id);

attr_selector* new_attr_name_selector(char* name);

attr_selector* new_attr_value_selector(char* name, int op, char* value);

filter_selector* new_filter(int filter);
#endif
