#ifndef __STACK_H__
#define __STACK_H__

struct list_bucket
{
    short type;
    void* element;
};

typedef struct generic_list_s{
  struct list_bucket** array;
  int start;
  int count;
  int capacity;
} list;

typedef struct generic_list_s stack;
typedef struct generic_list_s queue;

extern struct generic_list_s *new_generic_list(int initial);
extern list* new_generic_list(int initial);
extern stack* new_stack(int initial);
extern queue* new_queue(int initial);

extern void* set_element_with_type_at(list *l, void* obj, short type, int pos);
extern void* set_element_at(list *l, void* obj, int pos);

extern void insert_element_with_type_at(list* l, void* obj, short type, int pos);
extern void insert_element_at(list* l, void* obj, int pos);

extern void append_element(list* l, void* obj, short type);
extern void prepend_element(list* l, void* obj, short type);

extern void add_element_with_type(list* l, void* obj, short type);
extern void add_element(list* l, void* obj);

extern void* get_element_at(list* l, int pos);
extern void* get_element_and_type_at(list* l, int pos, short* type);

extern int remove_element(list *l, void* obj);
extern int remove_all(list *l, void* obj);
extern void* remove_element_at(list* l, int pos);

extern void enqueue_with_type(queue* q, void* obj, short type);
extern void enqueue(queue* q, void* obj);
extern void* dequeue(queue* q);

extern void push_stack_type(stack* s, void* obj, short type);
extern void push_stack(stack* s, void* obj);
extern void* pop_stack(stack* s);

extern void reverse(struct generic_list_s *s);

extern short peek_element_type_at(list* l, int pos);
extern short peek_stack_type(stack *s);
extern short peek_queue_type(queue *s);

extern struct generic_list_s *merge_lists(struct generic_list_s *l1, struct generic_list_s *l2);

extern void destroy_generic_list(struct generic_list_s *s);
#endif

