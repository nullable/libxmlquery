#include <stdint.h>
#include <string.h>
#include "../include/rbtree.h"
#include "../include/macros.h"
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

void set_symbol(char* id, void* value, void (*destruct_symbol)(void*symbol)){
  symbol* s = alloc(symbol, 1);
  s->id = strdup(id);
  s->value = strdup(value);
  s->destruct_symbol = destruct_symbol;
  rb_tree_insert(symbol_table, s);
}

void init_symbol_table(){
  symbol_table = new_rbtree(symbol_key, symbol_compare);
}

void destroy_symbol_table(){
  tree_iterator* it = new_tree_iterator(symbol_table);
  while(tree_iterator_has_next(it)){
    symbol *s = (symbol*) tree_iterator_next(it);
    s->destruct_symbol(s->value);
    free(s->id);
    free(s);
  }
  destroy_iterator(it);
  destroy_rbtree(symbol_table);
}
