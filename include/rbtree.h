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

#ifndef __RBTREE_H__
#define __RBTREE_H__

#define RED 1
#define BLACK 2

#include <stdint.h>
#include "macros.h"

typedef struct stree_node{
  void* node;

  uint8_t color;

  struct stree_node* parent;
  struct stree_node* left;
  struct stree_node* right;
}tree_node;

typedef struct sroot{
  struct stree_node* root;
  const void* (*key)(const struct stree_node* node);
  int64_t (*compare)(const void* keyA, const void* keyB);
}tree_root;

typedef struct siterator{
  struct stree_node* current;
}tree_iterator;

extern tree_root* new_simple_rbtree();
extern tree_root* new_rbtree(const void* (*key_function_pointer)(const struct stree_node* node),
			   int64_t (*compare_function_pointer)(const void* keyA, const void* keyB));
extern void* rb_tree_insert(tree_root* root, void* node);
extern void* rb_tree_delete(tree_root* root, void* key);
extern void* search_rbtree(const tree_root* root, const void* key);
extern void destroy_rbtree(tree_root* root);

extern tree_iterator* new_tree_iterator(tree_root* root);
extern uint8_t tree_iterator_has_next(tree_iterator* it);
extern void* tree_iterator_next(tree_iterator* it);
extern void destroy_iterator(tree_iterator* it);

extern const void* ident(const struct stree_node* o);
extern int64_t compare_string(const void* keyA, const void* keyB);
#endif

