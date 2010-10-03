#ifndef __STACK_H__
#define __STACK_H__
typedef struct stack_s{
  void** stack;
  int* type;
  int count;
  int capacity;
} stack;

extern stack* new_stack(int initial);

extern void push_stack_type(stack* s, void* obj, int type);

extern void push_stack(stack* s, void* obj);

extern void* pop_stack(stack* s);

extern void reverse_stack(stack* s);

extern int peek_stack_type(stack* s);

extern void destroy_stack(stack *s);
#endif
