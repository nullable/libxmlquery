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

typedef struct textnode_s{
  enum node_type type;
  struct snode* parent;
  char* value;
} textnode_dom_node;


typedef struct snode{
  enum node_type type;
  struct snode* parent;
  union{
      struct sroot* attributes;
      char* value;
  };

  char* name;
  char* namespace;

  struct generic_list_s* children;
}dom_node;

typedef struct sdoc{
  struct snode* root;
  list* xml_declarations;
}doc;

extern char* set_namespace(dom_node* node, const char* namespace);
extern char* set_name(dom_node* node, char* name);
extern char* set_value(dom_node* node, char* value);
extern dom_node* set_doc_root(doc* document, struct snode* root);
extern list* set_xml_declarations(doc* document, list* declarations);
extern void set_parent(dom_node* node, dom_node* parent);

extern char* get_namespace(dom_node* node);
extern char* get_name(dom_node* node);
extern char* get_value(dom_node* node);
extern list* get_xml_declarations(doc* document);
extern dom_node* get_doc_root(doc* document);

extern void prepend_child(dom_node* parent, dom_node* child);
extern void append_child(dom_node* parent, dom_node* child);
extern void append_children(dom_node* parent, struct generic_list_s* children);
extern void add_attribute(dom_node* node, dom_node* attribute);

extern doc* new_document(list* xml_declarations);
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
extern void destroy_dictionary();
#endif

