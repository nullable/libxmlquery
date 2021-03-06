/*
Copyright (c) 2010 Frederico Gonçalves, Vasco Fernandes

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
 
#ifndef __STACK_H__
#define __STACK_H__

#include <stdint.h>

struct list_bucket
{
    int16_t type;
    void* element;
};

typedef struct generic_list_s{
  void** array;
  int8_t with_type;
  int32_t start;
  int32_t count;
  int32_t capacity;
} list;

typedef struct generic_list_iterator_s{
  uint32_t pos;
  struct generic_list_s* list;
} generic_list_iterator;

typedef struct generic_list_s stack;
typedef struct generic_list_s queue;

extern uint8_t generic_list_is_empty(struct generic_list_s* l);
extern int32_t generic_list_get_count(struct generic_list_s* l);

extern struct generic_list_s *new_generic_list(int32_t initial);
extern struct generic_list_s *new_generic_list_with_type(int32_t initial);
extern stack* new_stack(int32_t initial);
extern queue* new_queue(int32_t initial);

extern void* set_element_with_type_at(list *l, void* obj, int16_t type, int32_t pos);
extern void* set_element_at(list *l, void* obj, int32_t pos);

extern void insert_element_with_type_at(list* l, void* obj, int16_t type, int32_t pos);
extern void insert_element_at(list* l, void* obj, int32_t pos);

extern void sorted_insert_element_with_type(list* l, void* obj, int16_t type, int(*compare)(void* o1, int16_t type1, void* o2, int16_t type2));

extern void append_element_with_type(list* l, void* obj, int16_t type);
extern void prepend_element_with_type(list* l, void* obj, int16_t type);

extern void add_element_with_type(list* l, void* obj, int16_t type);
extern void add_element(list* l, void* obj);

extern void* get_element_at(const list* l, int32_t pos);
extern void* get_element_with_type_at(const list* l, int32_t pos, int16_t* type);

extern int get_element_pos(const list* l, void* el);

extern int32_t remove_element(list *l, void* obj);
extern int32_t remove_all(list *l, void* obj);
extern void* remove_element_at(list* l, int32_t pos);

extern void enqueue_with_type(queue* q, void* obj, int16_t type);
extern void enqueue(queue* q, void* obj);
extern void* dequeue(queue* q);

extern void push_stack_type(stack* s, void* obj, int16_t type);
extern void push_stack(stack* s, void* obj);
extern void* pop_stack(stack* s);

extern list* remove_duplicates(list* l);

extern int16_t peek_element_type_at(list* l, int32_t pos);
extern int16_t peek_stack_type(stack *s);
extern int16_t peek_queue_type(queue *s);

extern struct generic_list_s *merge_lists(struct generic_list_s *l1, struct generic_list_s *l2);

extern struct generic_list_s *duplicate_generic_list(const struct generic_list_s*);
extern void destroy_generic_list(struct generic_list_s *s);

extern generic_list_iterator* new_generic_list_iterator(struct generic_list_s*);
extern uint8_t generic_list_iterator_has_next(generic_list_iterator* i);
extern void* generic_list_iterator_next(generic_list_iterator* i);
extern void destroy_generic_list_iterator(generic_list_iterator* i);
#endif

