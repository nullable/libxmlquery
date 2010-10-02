#include "../dom/macros.h"
#include "stack.h"

stack* new_stack(int initial){
  if(initial <= 0) exit(1);
  stack* s = alloc(stack, 1);
  s->count = 0;
  s->capacity = initial;
  
  s->stack = alloc(void*, initial);
  s->type = alloc(int, initial);
  return s;
}

void push_stack_type(stack* s, void* obj, int type){
  if(s->count == s->capacity){
    s->stack = (void*)realloc(s->stack, sizeof(void*) * s->capacity * 2);
    s->type = (void*)realloc(s->type, sizeof(int) * s->capacity * 2);
    s->capacity*=2;
  }
  s->stack[s->count] = obj;
  s->type[s->count] = type;
  s->count++;
}

void push_stack(stack* s, void* obj){
  push_stack_type(s, obj, -1);
}

void* pop_stack(stack* s){
  if(s->count == 0) { return NULL; }
  s->count--;
  return s->stack[s->count];
}

void reverse_stack(stack* s){
  void* tmp;
  int i;
  for(i = 0; i < s->count/2; i++){
    tmp = s->stack[i];
    s->stack[i] = s->stack[s->count-1-i];
    s->stack[s->count-1-i] = tmp;
  }
}

int peek_stack_type(stack* s){
  return s->type[s->count-1];
}

void destroy_stack(stack *s){
  if(s){
    free(s->type);
    free(s->stack);
    free(s);
  }
}
