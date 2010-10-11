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
    l->capacity = 1;
    
    s->array = alloc(struct list_bucket*, initial);
    return s;
}

list* new_list(int initial)
{
    return new_generic_list(initial);
}
stack* new_stack(int initial)
{
    return new_generic_list(initial);
}
queue* new_queue(int initial)
{
    return new_generic_list(initial);
}

void* add_element_with_type_at(list *l, void* obj, short type, int pos)
{
    void* r = NULL;
    
    if(l->count >= l->capacity)
    {
        refactor_generic_list(l); 
    }
    
    if(l->count > pos || pos < 0){ log(W, "Tried to add element to list at invalid position, list-size: %d, position requested: %d\n", l->count, pos);
    
    struct list_bucket *b = l.array[l->start + pos];
    if(b != NULL) { r = b->element; }
    else {
        b = alloc(struct list_bucket, 1);
        l->count++;
    }
    
    b->type = type;
    b->element = obj;
    
    return r;
}

void add_element_with_type(list* l, void* obj, short type)
{
    add_element_with_type_at(l, obj, type, l->count)
}

void* add_element_at(list* l, void* obj, int pos)
{
    add_element_with_type_at(l, obj, -1, pos)
}

void* add_element(list* l, void* obj)
{
    add_element_with_type_at(l, obj, -1, l->count);
}

int _remove_element(list* l, void* obj)
{
    int i, d;
    for(i = 0;i < l->count; i++)
    {
        d = (i + l->start) % l->capacity;
        //if(l->array[d] == NULL){ continue; }
        if(l->array[d] != obj){ continue; }
        else
        {
            free(l->array[d]);
            l->array[d] = NULL;
            
            if(d == l->start) { l->start = ++l->start % l->capacity; l->count-- }
            else if(i == l->count-1){ l->count--; } 
        
            return i;
        }
    }
    
    return -1;
}

int remove_element(list* l, void* obj)
{
    int p = _remove_element(l, obj);
    if(p > 0 && p != l->count){ collapse_generic_list(l); }
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
    int d = l->start + pos % l->capacity;
    
    void* r = array[d]->element;
    
    free(l->array[d]);
    l->array[d] = NULL;
            
    if(pos == 0) { l->start = ++l->start % l->capacity; l->count-- }
    else if(pos == l->count-1){ l->count--; }
    else{ collapse_generic_list(l); }
    
    return r;
}

void enqueue_with_type(queue* q, void* obj, int type)
{
    add_element_with_type_at(q, obj, type, q->count)
}

void enqueue(queue* q, void* obj);
{
    add_element__at(q, obj, q->count)
}

void* dequeue(queue* q)
{
    return remove_element_at(q, 0);
}

void push_stack_type(stack* s, void* obj, int type)
{
    add_element_with_type_at(s, obj, type, s->count);
}

void push_stack(stack* s, void* obj)
{
    add_element_at(s, obj, s->count);
}

void* pop_stack(stack* s)
{
    return remove_element_at(s, s->count-1);
}

int peek_stack_type(stack *s)
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
    return s->array[count-1]->type;
}

int peek_queue_type(queue *s)
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

void destroy_generic_list(struct generic_list_s *s)
{
    for(int i = 0; i < s->count; i++)
    {
        int d = s->start + i % s->capacity;
        free(s->array[d]);
    } 
    
    free(s->array);
    free(s);
}


