#ifndef __RBTREE_H__
#define __RBTREE_H__

#define RED 1
#define BLACK 2

#include "../dom/macros.h"

typedef struct stree_node{
  void* node;

  int color;
  
  struct stree_node* parent;
  struct stree_node* left;
  struct stree_node* right;
}tree_node;

typedef struct sroot{
  struct stree_node* root;
  void* (*key)(struct stree_node* node);
  int (*compare)(void* keyA, void* keyB);
}tree_root;

extern tree_root* new_tree(void* (*key_function_pointer)(struct stree_node* node),
			   int (*compare_function_pointer)(void* keyA, void* keyB));
extern void red_black_tree_insert(tree_root* root, void* node);
extern void red_black_tree_delete(tree_root* root, void* key);
extern void* search(tree_root root, void* key);
extern void destroy_tree(tree_root* root);

extern void print(tree_node* root, int pad);
#endif
