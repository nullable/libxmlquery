#ifndef __LIST_H__
#define __LIST_H__

#include "node.h"
#include "macros.h"

typedef struct slist_node{
  struct snode* node;
  struct slist_node* next;
}list_node;

typedef struct slist_keeper{
  struct slist_node* first;
  struct slist_node* last;
  unsigned int count;
}list_keeper;

typedef struct slist_iterator{
  struct slist_node* current;
}list_iterator;

extern list_keeper* new_list();
extern struct snode* pop(list_keeper* keeper);
extern void append(list_keeper* keeper, struct snode* node);
extern void add_all(list_keeper* l1, list_keeper* l2);
extern void prepend(list_keeper* keeper, struct snode* node);
extern struct snode* get(list_keeper keeper, int index);
extern list_keeper* get_by_name(list_keeper keeper, char* name);
extern list_keeper* get_by_namespace(list_keeper keeper, char* namespace);
extern list_keeper* regex_get_by_name(list_keeper keeper, char* pattern);
extern list_keeper* regex_get_by_namespace(list_keeper keeper, char* pattern);
extern list_keeper* regex_get_by_name_ignore_case(list_keeper keeper, char* pattern);
extern list_keeper* regex_get_by_namespace_ignore_case(list_keeper keeper, char* pattern);

extern list_iterator* new_list_iterator(list_keeper* k);
extern struct snode* list_iterator_next(list_iterator* it);
extern int list_iterator_has_next(list_iterator* it);
extern void destroy_list_iterator(list_iterator* it);

extern void destroy(list_keeper* keeper);
#endif
