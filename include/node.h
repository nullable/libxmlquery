#ifndef __NODE_H__
#define __NODE_H__

#include "rbtree.h"
#include "stack.h"

enum node_type {
  ATTRIBUTE,
  ELEMENT,
  TEXT_NODE,
  CDATA
};

typedef struct snode{
  enum node_type type;

  char* namespace;
  char* name;
  char* value;

  struct sroot* attributes;

  struct snode* parent;

  struct generic_list_s* children;
}dom_node;

typedef struct sdoc{
  struct snode* root;
  struct snode* xml_declaration;
}doc;

extern char* set_namespace(dom_node* node, const char* namespace);
extern char* set_name(dom_node* node, char* name);
extern char* set_value(dom_node* node, char* value);
extern dom_node* set_doc_root(doc* document, struct snode* root);
extern dom_node* set_xml_declaration(doc* document, struct snode* vers);
extern void set_parent(dom_node* node, dom_node* parent);

extern char* get_namespace(dom_node* node);
extern char* get_name(dom_node* node);
extern char* get_value(dom_node* node);
extern dom_node* get_xml_declaration(doc* document);
extern dom_node* get_doc_root(doc* document);

extern void prepend_child(dom_node* parent, dom_node* child);
extern void append_child(dom_node* parent, dom_node* child);
extern void append_children(dom_node* parent, struct generic_list_s* children);
extern void add_attribute(dom_node* node, dom_node* attribute);

extern doc* new_document(struct snode* xml_declaration);
extern dom_node* new_element_node(const char* name);
extern dom_node* new_text_node(char* text);
extern dom_node* new_attribute(char* name, char* value);
extern dom_node* new_cdata(char* cdata_text);

extern dom_node* get_child_at(dom_node* parent, int index);
extern struct generic_list_s* get_children(dom_node* node);
extern struct generic_list_s* get_descendants(dom_node* node);
extern struct generic_list_s* get_text_nodes(doc* root);
extern dom_node* get_attribute_by_name(dom_node* node, char* attr_name);
extern struct generic_list_s* get_elements_by_name(doc* root, char* name);

extern dom_node* delete_attribute(dom_node* node, char* name);
extern void remove_node(doc* root, dom_node* node);

extern void destroy_dom_node(dom_node* n);
extern void destroy_dom_tree(doc* root);
#endif

