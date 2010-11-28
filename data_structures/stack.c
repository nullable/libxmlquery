#include <stdio.h>
#include <string.h>
#include "../include/macros.h"
#include "../include/stack.h"

struct generic_list_s *_new_generic_list(int32_t initial, int8_t with_type)
{
  if(initial <= 0){
    log(F, "New generic list initial capacity must be greater than 0, %d given\n", initial);
    exit(1);
  }

  struct generic_list_s* l = alloc(struct generic_list_s, 1);
  l->start = 0;
  l->count = 0;
  l->capacity = initial;
  l->with_type = with_type;

  l->array = alloc(void*, initial);
  memset(l->array, 0, initial * sizeof(void**));
  return l;
}

struct generic_list_s* new_generic_list(int32_t initial){
    return _new_generic_list(initial, 0);
}

struct generic_list_s* new_generic_list_with_type(int32_t initial){
    return _new_generic_list(initial, 1);
}

uint8_t generic_list_is_empty(struct generic_list_s* l){
  return l->count == 0;
}

int32_t generic_list_get_count(struct generic_list_s* l){
  return l->count;
}

/*
static void rebase_generic_list(list* l){
    if(l->start == 0){ return ; }
    void** tmp;
    //apparently this function is never called !!!!
    //printf("[%d] + %d of %d\n", l->start, l->count, l->capacity);

    int cpy = l->count - l->capacity + l->start;

    if(cpy > 0){
        tmp = alloc(void*, cpy)

        memcpy(tmp, l->array, sizeof(void*)*cpy);
    }
    else{ cpy = 0; }

    //printf("mv %d + %d to 0\n", l->start, l->count - cpy);
    memcpy(l->array, l->array + l->start, sizeof(void*)*(l->count - cpy));

    if(cpy > 0){
        printf("mv + %d to %d\n", cpy, l->count - cpy);
        memcpy(l->array + l->count - cpy, tmp, sizeof(void*)*cpy);
        free(tmp);
    }

    l->start = 0;
}
*/

static void refactor_generic_list(list* l){
  int32_t cpy;

  l->array = realloc(l->array, sizeof(void*) * l->capacity * 2);
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

    if(!l->with_type && type != 0){ log(W, "Setting list element with type %d in a list without type, type will be lost", type);}

    if(!(pos < l->count && pos >= 0)){
        log(E, "Tried to set element the a list at an invalid position, list-size: %d, position requested: %d\n", l->count, pos);
        exit(-1);
    }


    if(l->with_type){
        struct list_bucket *b = l->array[(l->start + pos) % l->capacity];
        if(b != NULL) { r = b->element; }
        else {
            b = alloc(struct list_bucket, 1);
            l->array[(l->start + pos) % l->capacity] = b;
        }

        b->type = type;
        b->element = obj;
    }
    else{
        r = l->array[(l->start + pos) % l->capacity];
        l->array[(l->start + pos) % l->capacity] = obj;
    }

    return r;
}

void* set_element_at(list* l, void* obj, int32_t pos)
{
  return set_element_with_type_at(l, obj, -1, pos);
}

void insert_element_with_type_at(list* l, void* obj, int16_t type, int32_t pos){
    int32_t i;

    if(!l->with_type && type != 0){ log(W, "Inserting element with type %d in a list without type, type will be lost", type);}


    if(!(pos <= l->count && pos >= 0)){
        log(E, "Tried to insert an element to list at invalid position, list-size: %d, position requested: %d\n", l->count, pos);
        exit(-1);
    }

    if(l->count >= l->capacity){
        refactor_generic_list(l);
    }

    for(i = l->count - 1; i >= pos; i--){
        int32_t d = (l->start + i) % l->capacity;
        int32_t dn = (d+1) % l->capacity;
        l->array[dn] = l->array[d];
    }

    if(l->with_type){
        struct list_bucket* b = alloc(struct list_bucket, 1);
        b->type = type;
        b->element = obj;
        l->array[(l->start + pos) % l->capacity] = b;
    }
    else{
        l->array[(l->start + pos) % l->capacity] = obj;
    }

    l->count++;
}

void insert_element_at(list* l, void* obj, int32_t pos){
  insert_element_with_type_at(l, obj, 0, pos);
}

void sorted_insert_element_with_type(list* l, void* obj, int16_t type, int(*compare)(void* o1, int16_t type1, void* o2, int16_t type2)){
    int32_t i;
    int16_t type_v;

    for(i = 0; i < l->count; i++){
        void* v = get_element_with_type_at(l, i, &type_v);
        //printf("%d:%d <> %d:%d => %d\n", ((bc*)obj)->byte, ((bc*)obj)->count, ((bc*)v)->byte, ((bc*)v)->count, compare(obj, type, v, type_v));
        if(compare(obj, type, v, type_v) < 0) break;
    }

    insert_element_with_type_at(l, obj, type, i);
}

void append_element_with_type(list* l, void* obj, int16_t type)
{
  insert_element_with_type_at(l, obj, type, l->count);
}

void prepend_element_with_type(list* l, void* obj, int16_t type)
{
    if(!l->with_type && type != 0){ log(W, "Inserting element with type %d in a list without type, type will be lost", type);}

    if(l->count >= l->capacity){ refactor_generic_list(l); }
    if(--l->start < 0){ l->start = l->capacity - l->start; };

    if(l->with_type){
        struct list_bucket *b = alloc(struct list_bucket, 1);
        l->array[l->start] = b;
        b->type = type;
        b->element = obj;
    }
    else{
        l->array[l->start] = obj;
    }

    l->count++;
}


void add_element_with_type(list* l, void* obj, int16_t type)
{
  append_element_with_type(l, obj, type);
}

void add_element(list* l, void* obj)
{
  append_element_with_type(l, obj, 0);
}

int32_t _remove_element(list* l, void* obj)
{
    int32_t i, d;

    for(i = 0;i < l->count; i++)
    {
        d = (i + l->start) % l->capacity;

        if(get_element_at(l, i) == obj){
            if(l->with_type){
                free(l->array[d]);
            }
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

    void* r;

    if(l->with_type){
        r = ((struct list_bucket*)l->array[d])->element;

        free(l->array[d]);
        l->array[d] = NULL;
    }
    else{
        r = l->array[d];
    }

    if(pos == 0) {
        l->start = ++l->start % l->capacity;
        l->count--;
    }
    else if(pos == l->count-1){
        l->count--;
    }
    else{
        collapse_generic_list(l);
    }

    return r;
}

void* get_element_with_type_at(const list* l, int32_t pos, int16_t* type)
{
    if(pos < 0 || pos >= l->count){ log(W, "Trying to access object on position (%d) outside range (0 to %d).", pos, l->count-1); return NULL; }

    int32_t d = (l->start + pos) % l->capacity;

    void* r;

    if(l->with_type){
        r = ((struct list_bucket*)l->array[d])->element;
        if(type != NULL){ *type = ((struct list_bucket*)l->array[d])->type; }
    }
    else{
        if(type != NULL){ log(W, "Accessing type on a list without type, returned type is undefined.");}
        r = l->array[d];
    }
    return r;
}

void* get_element_at(const list* l, int32_t pos){
  return get_element_with_type_at(l, pos, NULL);
}

int get_element_pos(const list* l, void* el){
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
  if(!l->with_type) { log(W, "Peeking type on a list without type, returned type is ALWAYS 0."); return 0; }
  if(pos < 0 || pos >= l->count){ log(W, "Trying to access object on position (%d) outside range (0 to %d)", pos, l->count-1); exit(1); }
  int d = (l->start + pos) % l->capacity;
  return ((struct list_bucket*)l->array[d])->type;
}

int16_t peek_stack_type(stack *s)
{
    if(!s->with_type) { log(W, "Peeking type on a stack without type, returned type is ALWAYS 0."); return 0; }
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
    return ((struct list_bucket*)s->array[s->count-1])->type;
}

int16_t peek_queue_type(queue *s)
{
  if(!s->with_type) { log(W, "Peeking type on a queue without type, returned type is ALWAYS 0."); return 0; }
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
  return ((struct list_bucket*)s->array[s->start])->type;
}

struct generic_list_s *merge_lists(struct generic_list_s *l1, struct generic_list_s *l2)
{
    if(!l2) return l1;
    if(!l1) return l2;

    if(l1->with_type && !l2->with_type){ log(F, "List 1 has type information and list 2 does not, cannot merge."); exit(1); }
    if(!l1->with_type && l2->with_type){ log(F, "List 2 has type information and list 1 does not, cannot merge."); exit(1); }

    if(l1->count == 0){
        destroy_generic_list(l1);
        return l2;
    }
    else if(l2->count == 0){
        destroy_generic_list(l2);
        return l1;
    }

    int32_t i;
    int32_t new_count = l1->count + l2->count;

    struct generic_list_s *r = new_generic_list(new_count);

    int32_t n_d = 0, d;
    for(i = 0; i < l1->count; i++){
        d = (l1->start + i) % l1->capacity;
        r->array[n_d++] = l1->array[d];
    }

    for(i = 0; i < l2->count; i++){
        d = (l2->start + i) % l2->capacity;
        r->array[n_d++] = l2->array[d];
    }
    r->count = l1->count + l2->count;

    //current merge_list implementation depends on the containers of the source lists
    //therefor:
    l1->count = 0;
    l2->count = 0;

    //by placing count at 0 the destroy does not free those containers
    destroy_generic_list(l1);
    destroy_generic_list(l2);
    return r;
}

/*void __quicksort_generic_list_aux(struct list_bucket** arr, int start, int end) {
    if (start < end) {
        struct list_bucket* pivot = arr[end];
        int i = start;
        int j = end;
        while (i != j) {
            if ((arr[i]->element) < (pivot->element)) {
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
}*/

list* duplicate_generic_list(const list *l){
  if(!l) return NULL;
  list* new;
  if(l->with_type){
      new = new_generic_list_with_type(l->capacity);
  }
  else{
      new = new_generic_list(l->capacity);
  }

  int i;
  int16_t type;
  for(i = 0; i < l->count; i++)
    if(l->with_type){
      insert_element_with_type_at(new, get_element_with_type_at(l, i, &type), type, i);
    }
    else{
      insert_element_at(new, get_element_at(l, i), i);
    }

  return new;
}


list* exclude_list_from_list(const list* l1, const list* l2){
    if(l2 == NULL || l2->count == 0) return NULL;
    if(l1 == NULL || l1->count == 0) return duplicate_generic_list(l2);

    int32_t i;

    list* r = new_generic_list(l2->capacity + l1->capacity);

    int32_t pos;
    int16_t _type;
    int16_t* type = (l2->with_type)? &_type : NULL;

    void* obj;

    for(i = 0; i < l2->count; i++){
        obj = get_element_with_type_at(l2, i, type);
        pos = get_element_pos(l1, obj);
        if(pos > 0) append_element_with_type(r, obj, (l2->with_type)? *type : 0);
    }

    return r;
}


void destroy_generic_list(struct generic_list_s *s)
{
  int32_t i, d;
  if(s == NULL)
    return;

  if(s->with_type){
    for(i = 0; i < s->count; i++)
      {
        d = (s->start + i) % s->capacity;
        free(s->array[d]);
      }
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
  return (i->list == NULL)? 0 : (i->pos < i->list->count);
}

void* generic_list_iterator_next(generic_list_iterator* i){
  return get_element_at(i->list, (i->pos)++);
}

void destroy_generic_list_iterator(generic_list_iterator* i){
  free(i);
}

