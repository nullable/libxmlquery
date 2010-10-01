#ifndef __QUERY_PARSER_H__
#define __QUERY_PARSER_H__



struct selector_s;

typedef struct stack_s{
  void** stack;
  int* type;
  int count;
  int capacity;
} stack;

typedef struct attr_selector_s{
  char* name;
  int op;
  char* value;
} attr_selector;

typedef struct step_s{ int multiplier; int offset; } step;

typedef struct filter_selector_s{
  int op;
  union {
    step s;
    struct selector_s* selector;
  } value;
} filter_selector;

// typedef struct filter_array_s{
//   filter_selector** array;
//   int count;
//   int capacity;
// } filter_array;

typedef struct selector_s{
  char* id;
  stack* attrs;
  stack* filters;
} selector;

stack* new_stack(){
  stack* r = alloc(stack, 1);
  r->stack = alloc(void*, 1);
  r->type = alloc(int, 1);
  r->count = 0;
  r->capacity = 1;
  
  return 1;
}

void push_stack_type(stack* s, void* obj, int type);

void push_stack(stack* s, void* obj);

void* pop_stack(stack* s);

void reverse_stack(stack* s);

int peek_stack_type(stack* s);

selector* new_selector(char* id);

attr_selector* new_attr_name_selector(char* name);

attr_selector* new_attr_value_selector(char* name, int op, char* value);

filter_selector* new_filter(int filter);

filter_array new_filter_array();

void add_attr_selector(selector* s, attr_selector* a);

void add_filter_selector(filter_array* s, filter_selector* f);

#endif
