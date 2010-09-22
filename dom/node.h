#ifndef __NODE_H__
#define __NODE_H__

struct stree_node;
struct slist_keeper;


enum node_type {
  attribute,
  element,
  text
};

typedef struct snode{
  enum node_type type;

  char* name;
  char* value;

  struct stree_node attributes;
  
  struct snode* parent;
  
  struct slist_keeper children;
}dom_node;

#endif
