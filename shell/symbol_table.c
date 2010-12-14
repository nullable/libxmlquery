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
  symbol* n = (symbol*) search_rbtree(symbol_table, id);

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
