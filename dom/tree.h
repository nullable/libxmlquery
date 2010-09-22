#ifndef __TREE_H__
#define __TREE_H__

#define key(NODE)\
  (NODE->node->name)

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


#endif
