#ifndef __QUERY_PARSER_H__
#define __QUERY_PARSER_H__

#define LXQ_RELATION_TYPE 0
#define LXQ_SELECTOR_TYPE 1

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

extern selector* new_selector(char* id);
extern attr_selector* new_attr_value_selector(char* name, int op, char* value);
extern filter_selector* new_filter(int filter);

extern void destroy_selector(selector* s);
extern void destroy_filter_selector(filter_selector* fs);
extern void destroy_attr_selector(attr_selector* as);
#endif

