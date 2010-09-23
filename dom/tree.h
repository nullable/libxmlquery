#ifndef __TREE_H__
#define __TREE_H__

#include <string.h>

#define key(NODE)\
  (NODE->node->name)

#define compare(NODE_A, NODE_B)\
  (strcmp(key(NODE_A), key(NODE_B)))

#define RED 1
#define BLACK 2

struct snode;

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

tree_node NIL = {
  .node = NULL,
  .color = BLACK,
  .parent = NULL,
  .left = NULL,
  .right = NULL
};

extern void red_black_tree_insert(tree_root* root, struct snode* node);
#endif
