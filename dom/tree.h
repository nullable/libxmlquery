#ifndef __TREE_H__
#define __TREE_H__

#include <string.h>

#define key(NODE)\
  (NODE->node->name)

#define compare(KEY_A, KEY_B)\
  (strcmp(KEY_A, KEY_B))

#define RED 1
#define BLACK 2

#include "list.h"
#include "node.h"
#include "macros.h"

typedef struct stree_node{
  struct snode* node;

  int color;
  
  struct stree_node* parent;
  struct stree_node* left;
  struct stree_node* right;
}tree_node;

typedef struct sroot{
  struct stree_node* root;
}tree_root;

extern void red_black_tree_insert(tree_root* root, struct snode* node);
extern struct slist_keeper* search(tree_root root, const char* name);
#endif
