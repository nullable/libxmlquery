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

typedef struct siterator{
  struct stree_node* current;
}tree_iterator;

extern tree_root* new_tree();
extern void red_black_tree_insert(tree_root* root, struct snode* node);
extern struct snode* search(tree_root root, const char* name);
extern void destroy_tree(tree_root* root);
extern struct slist_keeper* regex_search(tree_root root, char* pattern);
extern struct slist_keeper* regex_search_ignore_case(tree_root root, char* pattern);

extern tree_iterator* new_tree_iterator(tree_root* root);
extern int tree_iterator_has_next(tree_iterator* it);
extern struct snode* tree_iterator_next(tree_iterator* it);
extern void destroy_iterator(tree_iterator* it);
#endif
