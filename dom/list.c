#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "tree.h"
#include "node.h"
#include "macros.h"

list_keeper* new_list(){
  list_keeper * lk = alloc(list_keeper, 1);
  lk->first = NULL;
  lk->last = NULL;
  lk->count = 0;
  return lk;
}

void append(list_keeper* keeper, struct snode* node){
  list_node* lnode;
  
  if(keeper == NULL){
    log(W, "Trying to append node to null keeper.");
    return;
  }

  lnode = alloc(list_node, 1);
  lnode->node = node;
  lnode->next = NULL;

  keeper->count++;

  if(keeper->first == NULL){
    keeper->first = lnode;
    keeper->last = lnode;
    return;
  }

  keeper->last->next = lnode;
  keeper->last = lnode;
  return;
}

struct snode* get(list_keeper keeper, int index){
  unsigned int it;
  list_node* lit;

  if(index < 0 || keeper.count <= index){
    log(W, "Index out of bounds.");
    return NULL;
  }

  for(it = 0, lit = keeper.first; it < index; it++, lit = lit->next);

  return lit->node;
}

list_keeper* get_by_name(list_keeper keeper, char* name){
  list_keeper* list = new_list();
  list_node* lit;

  for(lit = keeper.first; lit != NULL; lit = lit->next){
    if(strcmp(lit->node->name, name) == 0)
      append(list, lit->node);
  }

  return list;
}

void destroy(list_keeper* keeper){
  list_node* lit;

  lit = keeper->first;
  while(lit != NULL){
    keeper->first = lit->next;
    free(lit);
    lit = keeper->first;
  }

  free(keeper);
}
