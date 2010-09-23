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

extern list_keeper* new_list();
extern void append(list_keeper* keeper, struct snode* node);
extern struct snode* get(list_keeper keeper, int index);
extern list_keeper* get_by_name(list_keeper keeper, char* name);

extern void destroy(list_keeper* keeper);
#endif
