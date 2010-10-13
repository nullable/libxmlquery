#include <string.h>
#include "../dom/macros.h"
#include "stack.h"

struct generic_list_s *new_generic_list(int initial)
{
  if(initial <= 0){
    log(F, "New generic list initial capacity must be greater than 0, %d given\n", initial);
    exit(1);
  }
  
  struct generic_list_s* l = alloc(struct generic_list_s, 1); 
  l->start = 0;
  l->count = 0;
  l->capacity = initial;
  
  l->array = alloc(struct list_bucket*, initial);
  return l;
}

static void refactor_generic_list(list* l){
  int cpy;

  l->array = realloc(l->array, l->capacity * 2);
  if(l->array == NULL){
    log(F, "Realloc failed.\n");
    exit(-1);
  }

  cpy = l->count - l->capacity + l->start;

  if(cpy > 0){
    memcpy(l->array + l->capacity, l->array, cpy);
    memset(l->array, 0, cpy);
  }

  l->capacity *= 2;
}

stack* new_stack(int initial)
{
  return new_generic_list(initial);
}
queue* new_queue(int initial)
{
  return new_generic_list(initial);
}

void* set_element_with_type_at(list *l, void* obj, short type, int pos)
{
  void* r = NULL;
  
  if(l->count >= l->capacity)
    {
      refactor_generic_list(l); 
    }
  
  if(pos >= l->count || pos < 0) log(W, "Tried to add element to list at invalid position, list-size: %d, position requested: %d\n", l->count, pos);
    
  struct list_bucket *b = l->array[(l->start + pos) % l->capacity];
  if(b != NULL) { r = b->element; }
  else {
    b = alloc(struct list_bucket, 1);
    l->count++;
    l->array[(l->start + pos) % l->capacity] = b;
  }
  
  b->type = type;
  b->element = obj;
  
  return r;
}

void* set_element_at(list* l, void* obj, int pos)
{
  return set_element_with_type_at(l, obj, -1, pos);
}

void append_element(list* l, void* obj, short type)
{
  if(l->count >= l->capacity)
  {
    refactor_generic_list(l); 
  }
  
  struct list_bucket *b = alloc(struct list_bucket, 1);
  l->array[(l->start + l->count) % l->capacity] = b;
  l->count++;

  b->type = type;
  b->element = obj;    
}

void prepend_element(list* l, void* obj, short type)
{
  if(l->count >= l->capacity)
  {
    refactor_generic_list(l); 
  }

  struct list_bucket *b = alloc(struct list_bucket, 1);
  if(--l->start < 0){ l->start = l->capacity - l->start; };
  
  l->array[l->start] = b;
  l->count++;

  b->type = type;
  b->element = obj; 
}


void add_element_with_type(list* l, void* obj, short type)
{
  append_element(l, obj, type);
}

void add_element(list* l, void* obj)
{
  append_element(l, obj, -1);
}

int _remove_element(list* l, void* obj)
{
  int i, d;
  for(i = 0;i < l->count; i++)
    {
      d = (i + l->start) % l->capacity;
      
      if(l->array[d] != obj){ 
	continue; 
      }
      else
        {
	  free(l->array[d]);
	  l->array[d] = NULL;
          
	  if(d == l->start) { l->start = ++l->start % l->capacity; l->count--; }
	  else if(i == l->count-1){ l->count--; } 
	  
	  return i;
        }
    }
  
  return -1;
}

static void collapse_generic_list(list* l){
  int i = 0, c = 0, d, e;
  for(; i < l->count; i++){
    d = (l->start + i) % l->capacity;
    if(l->array[d] == NULL){
      c++;
    }
    else if(c > 0){
      e = d-c;
      if(e < 0) e = l->capacity - e;
      l->array[e] = l->array[d];
    }
  }
  l->count -= c;
}

int remove_element(list* l, void* obj)
{
  int p = _remove_element(l, obj);
  if(p > 0 && p != l->count){
    collapse_generic_list(l); 
  }
  return p;
}

int remove_all(list* l, void* obj)
{
  int c = 0;
  while(_remove_element(l, obj) == -1){ c++; }
  if(c > 0){ collapse_generic_list(l); }
  
  return c;
}

void* remove_element_at(list* l, int pos)
{
  if(pos >= l->count){ log(W, "Trying to remove object on position (%d) greater than element count (%d)", pos, l->count); return NULL; }
  int d = (l->start + pos) % l->capacity;
  
  void* r = l->array[d]->element;
  
  free(l->array[d]);
  l->array[d] = NULL;
  
  if(pos == 0) { 
    l->start = ++l->start % l->capacity; 
    l->count--; 
  }
  else 
    if(pos == l->count-1)
      { l->count--; }
  else{ collapse_generic_list(l); }
  
  return r;
}

void* get_element_and_type_at(list* l, int pos, short* type)
{
  if(pos < 0 || pos >= l->count){ log(W, "Trying to access object on position (%d) outside range (0 to %d)", pos, l->count-1); return NULL; }
  int d = (l->start + pos) % l->capacity;
  void* r = l->array[d]->element;
  *type = l->array[d]->type;
  return r;
}

void* get_element_at(list* l, int pos){
  short type;
  return get_element_and_type_at(l, pos, &type);
}

void enqueue_with_type(queue* q, void* obj, short type)
{
  add_element_with_type(q, obj, type);
}

void enqueue(queue* q, void* obj)
{
  add_element(q, obj);
}

void* dequeue(queue* q)
{
  return remove_element_at(q, 0);
}

void push_stack_type(stack* s, void* obj, short type)
{
  add_element_with_type(s, obj, type);
}

void push_stack(stack* s, void* obj)
{
  add_element(s, obj);
}

void* pop_stack(stack* s)
{
  return remove_element_at(s, s->count-1);
}

short peek_element_type_at(list* l, int pos)
{
  if(pos < 0 || pos >= l->count){ log(W, "Trying to access object on position (%d) outside range (0 to %d)", pos, l->count-1); exit(1); }
  return l->array[(l->start + pos) % l->capacity]->type;
}

short peek_stack_type(stack *s)
{
  if(s == NULL)
    {
      log(F, "Stack is not initialized");
      exit(1);
    }
  
  if(s->count == 0)
    {
      log(F, "Stack is empty");
      exit(1);
    }
  return s->array[s->count-1]->type;
}

short peek_queue_type(queue *s)
{
  if(s == NULL)
    {
      log(F, "Queue is not initialized");
      exit(1);
    }
  
  if(s->count == 0)
    {
      log(F, "Queue is empty");
      exit(1);
    }
  return s->array[0]->type;
}

struct generic_list_s *merge_lists(struct generic_list_s *l1, struct generic_list_s *l2)
{
  int new_count = l1->count + l2->count, i;
  struct generic_list_s *r = new_generic_list(new_count);
  
  int n_d = 0, d;
  for(i = 0; i < l1->count; i++)
  {
    d = (l1->start + i) % l1->capacity;
    r->array[n_d++] = l1->array[d]
  }
  
  for(i = 0; i < l2->count; i++)
  {
    d = (l2->start + i) % l2->capacity;
    r->array[n_d++] = l2->array[d]
  }
  
  r->count = l1->count + l2->count;
  destroy_generic_list(l1);
  destroy_generic_list(l2);
  return r;
}

void destroy_generic_list(struct generic_list_s *s)
{
  int i, d;
  for(i = 0; i < s->count; i++)
    {
      d = (s->start + i) % s->capacity;
      free(s->array[d]);
    } 
  
  free(s->array);
  free(s);
}


