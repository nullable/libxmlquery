#ifndef __NODE_H__
#define __NODE_H__

#include "tree.h"
#include "list.h"

enum node_type {
  attribute,
  element,
  text,
  cdata
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

typedef struct sdoc{
  struct snode* root;
  char* xml_version;
}doc;

extern void set_namespace(dom_node* node, char* namespace);
extern void set_name(dom_node* node, char* name);
extern void set_value(dom_node* node, char* value);
extern void set_doc_root(doc* document, struct snode* root);
extern void set_xml_version(doc* document, char* vers);

extern char* get_namespace(dom_node* node);
extern char* get_name(dom_node* node);
extern char* get_value(dom_node* node);
extern char* get_xml_version(doc* document);
extern dom_node* get_doc_root(doc* document);

extern void prepend_child(dom_node* parent, dom_node* child);
extern void append_child(dom_node* parent, dom_node* child);
extern void add_attribute(dom_node* node, dom_node* attribute);

extern doc* new_document(char* xml_version);
extern dom_node* new_element_node(char* name);
extern dom_node* new_text_node(char* text);
extern dom_node* new_attribute(char* name, char* value);
extern dom_node* new_cdata(char* cdata_text);

extern dom_node* get_child_at(dom_node* parent, int index);
extern list_keeper* get_children(dom_node* node);

extern list_keeper* regex_get_attributes(dom_node* node, char* pattern);
extern list_keeper* regex_get_attributes_ignore_case(dom_node* node, char* pattern);
extern list_keeper* regex_get_elements_by_name(doc* root, char* pattern);
extern list_keeper* regex_get_elements_by_name_ignore_case(doc* root, char* pattern);
extern list_keeper* regex_get_elements_by_namespace(doc* root, char* pattern);
extern list_keeper* regex_get_elements_by_namespace_ignore_case(doc* root, char* pattern);
#endif
