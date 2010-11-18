#include <stdio.h>
#include <string.h>
#include "../include/macros.h"
#include "../include/stack.h"

struct generic_list_s *new_generic_list(int32_t initial)
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

uint8_t generic_list_is_empty(struct generic_list_s* l){
  return l->count == 0;
}

int32_t generic_list_get_count(struct generic_list_s* l){
  return l->count;
}

static void rebase_generic_list(list* l){
    if(l->start == 0){ return ; }
    struct list_bucket** tmp;
    printf("[%d] + %d of %d\n", l->start, l->count, l->capacity);

    int cpy = l->count - l->capacity + l->start;

    if(cpy > 0){
        tmp = alloc(struct list_bucket*, cpy)

        memcpy(tmp, l->array, sizeof(struct list_bucket*)*cpy);
    }
    else{ cpy = 0; }

    printf("mv %d + %d to 0\n", l->start, l->count - cpy);
    memcpy(l->array, l->array + l->start, sizeof(struct list_bucket*)*(l->count - cpy));

    if(cpy > 0){
        printf("mv + %d to %d\n", cpy, l->count - cpy);
        memcpy(l->array + l->count - cpy, tmp, sizeof(struct list_bucket*)*cpy);
        free(tmp);
    }

    l->start = 0;
}

static void refactor_generic_list(list* l){
  int32_t cpy;

  l->array = realloc(l->array, sizeof(struct list_bucket*) * l->capacity * 2);
  if(l->array == NULL){
    log(F, "Realloc failed.\n");
    exit(-1);
  }

  //number of items behind start (circular!)
  cpy = l->count - l->capacity + l->start;

  if(cpy > 0){

    memcpy(l->array + l->capacity, l->array, cpy);
    memset(l->array, 0, cpy);
  }

  l->capacity *= 2;
}

stack* new_stack(int32_t initial)
{
  return new_generic_list(initial);
}
queue* new_queue(int32_t initial)
{
  return new_generic_list(initial);
}

void* set_element_with_type_at(list *l, void* obj, int16_t type, int32_t pos)
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

void* set_element_at(list* l, void* obj, int32_t pos)
{
  return set_element_with_type_at(l, obj, -1, pos);
}

void insert_element_with_type_at(list* l, void* obj, int16_t type, int32_t pos){
  int32_t i;
  if(l->count >= l->capacity){
    refactor_generic_list(l);
  }

  for(i = l->count; i >= pos; i--){
    int32_t d = (l->start + i) % l->capacity;
    int32_t dn = (d+1) % l->capacity;
    l->array[dn] = l->array[d];
  }

  struct list_bucket* b = alloc(struct list_bucket, 1);
  b->type = type;
  b->element = obj;

  l->count++;
  l->array[(l->start + pos) % l->capacity] = b;
}

void insert_element_at(list* l, void* obj, int32_t pos){
  insert_element_with_type_at(l, obj, -1, pos);
}

#include "../include/huffman.h"
void sorted_insert_element_with_type_at(list* l, void* obj, int16_t type, int(*compare)(void* o1, int16_t type1, void* o2, int16_t type2)){
    int32_t i;
    int16_t type_v;

    for(i = 0; i < l->count; i++){
        void* v = get_element_and_type_at(l, i, &type_v);
        //printf("%d:%d <> %d:%d => %d\n", ((bc*)obj)->byte, ((bc*)obj)->count, ((bc*)v)->byte, ((bc*)v)->count, compare(obj, type, v, type_v));
        if(compare(obj, type, v, type_v) < 0) break;
    }

    insert_element_with_type_at(l, obj, type, i);
}

void append_element(list* l, void* obj, int16_t type)
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

void prepend_element(list* l, void* obj, int16_t type)
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


void add_element_with_type(list* l, void* obj, int16_t type)
{
  append_element(l, obj, type);
}

void add_element(list* l, void* obj)
{
  append_element(l, obj, -1);
}

int32_t _remove_element(list* l, void* obj)
{
  int32_t i, d;
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
  int32_t i = 0, c = 0, d, e;
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

int32_t remove_element(list* l, void* obj)
{
  int32_t p = _remove_element(l, obj);
  if(p > 0 && p != l->count){
    collapse_generic_list(l);
  }
  return p;
}

int32_t remove_all(list* l, void* obj)
{
  int32_t c = 0;
  while(_remove_element(l, obj) == -1){ c++; }
  if(c > 0){ collapse_generic_list(l); }

  return c;
}

void* remove_element_at(list* l, int32_t pos)
{
  if(pos >= l->count){ log(W, "Trying to remove object on position (%d) greater than element count (%d)", pos, l->count); return NULL; }
  int32_t d = (l->start + pos) % l->capacity;

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

void* get_element_and_type_at(list* l, int32_t pos, int16_t* type)
{
  if(pos < 0 || pos >= l->count){ log(W, "Trying to access object on position (%d) outside range (0 to %d)", pos, l->count-1); return NULL; }
  int32_t d = (l->start + pos) % l->capacity;
  void* r = l->array[d]->element;
  *type = l->array[d]->type;
  return r;
}

void* get_element_at(list* l, int32_t pos){
  int16_t type;
  return get_element_and_type_at(l, pos, &type);
}

int get_element_pos(list* l, void* el){
    int i;
    if(l == NULL) return -1;

    for(i = 0; i < l->count; i++){
        if(get_element_at(l, i) == el) return i;
    }

    return -1;
}

void enqueue_with_type(queue* q, void* obj, int16_t type)
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

void push_stack_type(stack* s, void* obj, int16_t type)
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

int16_t peek_element_type_at(list* l, int32_t pos)
{
  if(pos < 0 || pos >= l->count){ log(W, "Trying to access object on position (%d) outside range (0 to %d)", pos, l->count-1); exit(1); }
  return l->array[(l->start + pos) % l->capacity]->type;
}

int16_t peek_stack_type(stack *s)
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

int16_t peek_queue_type(queue *s)
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
  return s->array[s->start]->type;
}

struct generic_list_s *merge_lists(struct generic_list_s *l1, struct generic_list_s *l2)
{
  if(!l2) return l1;
  if(!l1) return l2;

  if(l1->count == 0){
    destroy_generic_list(l1);
    return l2;
  }else
    if(l2->count == 0){
      destroy_generic_list(l2);
      return l1;
    }

  int32_t new_count = l1->count + l2->count, i;
  struct generic_list_s *r = new_generic_list(new_count);

  int32_t n_d = 0, d;
  for(i = 0; i < l1->count; i++)
  {
    d = (l1->start + i) % l1->capacity;
    r->array[n_d++] = l1->array[d];
  }

  for(i = 0; i < l2->count; i++)
  {
    d = (l2->start + i) % l2->capacity;
    r->array[n_d++] = l2->array[d];
  }
  r->count = l1->count + l2->count;

  //current merge_list implementation depends on the containers of the source lists
  //therefor:
  l1->count = 0;
  l2->count = 0;

  //do that destroy does not free those containers
  destroy_generic_list(l1);
  destroy_generic_list(l2);
  return r;
}

void __quicksort_generic_list_aux(struct list_bucket** arr, int start, int end) {
    if (start < end) {
        struct list_bucket* pivot = arr[end];
        int i = start;
        int j = end;
        while (i != j) {
            if ((int)(arr[i]->element) < (int)(pivot->element)) {
                i = i + 1;
            }
            else {
                arr[j] = arr[i];
                arr[i] = arr[j-1];
                j = j - 1;
            }
        }
        arr[j] = pivot;
        __quicksort_generic_list_aux(arr, start, j-1);
        __quicksort_generic_list_aux(arr, j+1, end);
    }
}

void __quicksort_generic_list(list* l) {
    __quicksort_generic_list_aux(l->array, 0, l->count - 1);
  }

list* duplicate_generic_list(list *l){
  list* new = new_generic_list(l->capacity);
  int i;
  for(i = 0; i < l->count; i++)
    insert_element_at(new, get_element_at(l, i), i);
  return new;
}

list* remove_duplicates(list* l){
  if(l == NULL)
    return NULL;
  if(l->count <= 1)
    return l;

  int i;
  list* r = new_generic_list(l->capacity);
  rebase_generic_list(l);
  __quicksort_generic_list(l);
  add_element(r, get_element_at(l, 0));
  for(i = 1; i < l->count; i++){
    if(get_element_at(l, i-1) != get_element_at(l, i)){
      add_element(r, get_element_at(l, i));
    }
  }

  destroy_generic_list(l);
  return r;
}

void destroy_generic_list(struct generic_list_s *s)
{
  int32_t i, d;
  if(s == NULL)
    return;

  for(i = 0; i < s->count; i++)
    {
      d = (s->start + i) % s->capacity;
      free(s->array[d]);
    }

  free(s->array);
  free(s);
}

generic_list_iterator* new_generic_list_iterator(struct generic_list_s* l){
  generic_list_iterator* i = alloc(generic_list_iterator, 1);
  i->pos = 0;
  i->list = l;
  return i;
}

uint8_t generic_list_iterator_has_next(generic_list_iterator* i){
  return i->pos < i->list->count;
}

void* generic_list_iterator_next(generic_list_iterator* i){
  return get_element_at(i->list, (i->pos)++);
}

void destroy_generic_list_iterator(generic_list_iterator* i){
  free(i);
}

