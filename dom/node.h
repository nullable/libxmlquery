#ifndef __NODE_H__
#define __NODE_H__

#include "tree.h"
#include "list.h"

enum node_type {
  attribute,
  element,
  text
};

typedef struct snode{
  enum node_type type;

  char* namespace;
  char* name;
  char* value;

  struct stree_root* attributes;
  
  struct snode* parent;
  
  struct slist_keeper* children;
}dom_node;

#endif
