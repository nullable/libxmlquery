#include <stdint.h>
#include <string.h>
#include "../data_structures/rbtree.h"
#include "../dom/macros.h"
#include "symbol_table.h"

void* symbol_key(tree_node* node){
  return ((symbol*) node->node)->id;
}

int64_t symbol_compare(void* keyA, void* keyB){
  return strcmp((char*) keyA, (char*) keyB);
}

tree_root* symbol_table = NULL;

void* get_symbol(char* id){
  symbol* n = (symbol*) search_rbtree(*symbol_table, id);

  if(!n)
    return n;

  return n->value;
}

void set_symbol(char* id, void* value){
  symbol* s = alloc(symbol, 1);
  s->id = strdup(id);
  s->value = strdup(value);
  rb_tree_insert(symbol_table, s);
}

void init_symbol_table(){
  symbol_table = new_rbtree(symbol_key, symbol_compare);
}
