#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "macros.h"
#include "node.h"

void set_namespace(dom_node* node, char* namespace){
  int size;

  if(namespace == NULL)
    return;

  size = strlen(namespace) + 1;

  if(node->namespace != NULL){
    free(node->namespace);
  }

  node->namespace = alloc(char, size);
  strncpy(node->namespace, namespace, size);
}

void set_name(dom_node* node, char* name){
  int size;

  size = strlen(name) + 1;

  if(node->name != NULL){
    free(node->name);
  }

  node->name = alloc(char, size);
  strncpy(node->name, name, size);
}

void set_value(dom_node* node, char* value){
  int size = strlen(value) + 1;

  if(node->value != NULL){
    free(node->value);
  }

  node->value = alloc(char, size);
  strncpy(node->value, value, size);
}

void set_doc_root(doc* document, struct snode* root){
  document->root = root;
}

void set_xml_declaration(doc* document, dom_node* vers){
  document->xml_declaration = vers;
}

void set_parent(dom_node* node, dom_node* parent){
  node->parent = parent;
}

char* get_namespace(dom_node* node){
  return node->namespace;
}

char* get_name(dom_node* node){
  return node->name;
}

char* get_value(dom_node* node){
  return node->value;
}

dom_node* get_doc_root(doc* document){
  return document->root;
}

void prepend_child(dom_node* parent, dom_node* child){
  if(child == NULL) {
    log(W, "Trying to prepend a NULL child.\n");
    return;
  }
  if(child->type == ATTRIBUTE){
    log(W, "Trying to prepend attribute %s as child of node %s.\n", child->name, parent->name);
    return;
  }
  if(parent->children == NULL)
    parent->children = new_generic_list(16);

  log(W, "prepend_child needs to be implemented.\n");

  prepend_element(parent->children, child, 0);
  child->parent = parent;
}

void append_child(dom_node* parent, dom_node* child){
  if(child == NULL) {
    log(W, "Trying to append a NULL child.\n");
    return;
  }
  if(child->type == ATTRIBUTE){
    log(W, "Trying to append attribute %s as child of node %s.\n", child->name, parent->name);
    return;
  }
  if(parent->children == NULL)
    parent->children = new_generic_list(16);
  append_element(parent->children, child, 0);
  child->parent = parent;
}

void* key(struct stree_node* node){
  return ((dom_node*) node->node)->name;
}

int64_t compare(void* keyA, void* keyB){
  return strcmp((char*) keyA, (char*) keyB);
}

void add_attribute(dom_node* node, dom_node* attribute){
  if(attribute == NULL) {
    log(W, "Trying to add a NULL attrubute.\n");
    return;
  }
  if(attribute->type == ATTRIBUTE){
    if(node->attributes == NULL)
      node->attributes = new_rbtree(&key, &compare);
    rb_tree_insert(node->attributes, attribute);
    return;
  }
  log(W, "Trying to add node %s as attribute of node %s\n", attribute->name, node->name);
  return;
}

void append_children(dom_node* parent, struct generic_list_s* children){
  int i;
  if(children == NULL){
    log(W, "Trying to append NULL children.\n");
    return;
  }
  if(parent->children == NULL)
    parent->children = new_generic_list(16);

  for(i = 0; i < children->count; i++){
    append_child(parent, get_element_at(children, i));
  }
  //destroy_generic_list(children);
  return;
}

doc* new_document(dom_node* xml_declaration){
  doc* document = alloc(doc, 1);
  set_xml_declaration(document, xml_declaration);
  return document;
}

dom_node* new_element_node(char* name){
  dom_node* node = alloc(dom_node, 1);
  int name_size = strlen(name) + 1;

  node->type = ELEMENT;
  node->name = alloc(char, name_size);
  strncpy(node->name, name, name_size);
  node->value = NULL;
  node->namespace = NULL;
  node->attributes = NULL;
  node->children = NULL;
  return node;
}

dom_node* new_text_node(char* text){
  dom_node* node = alloc(dom_node, 1);
  int text_size = strlen(text) + 1;

  node->type = TEXT_NODE;
  node->name = "~#TEXT_NODE#~";
  node->value = alloc(char, text_size);
  strncpy(node->value, text, text_size);
  node->namespace = NULL;
  node->attributes = NULL;
  node->children = NULL;
  return node;
}

dom_node* new_attribute(char* name, char* value){
  dom_node* node = alloc(dom_node, 1);
  int name_size = strlen(name) + 1;
  int value_size = strlen(value) + 1;

  node->type = ATTRIBUTE;
  node->name = alloc(char,name_size);
  strncpy(node->name, name, name_size);
  node->value = alloc(char, value_size);
  strncpy(node->value, value, value_size);
  node->namespace = NULL;
  node->attributes = NULL;
  node->children = NULL;
  return node;
}

dom_node* new_cdata(char* cdata_text){
  dom_node* node = alloc(dom_node, 1);
  int cdata_size = strlen(cdata_text) + 1;

  node->type = CDATA;
  node->name = "~#CDATA#~";
  node->value = alloc(char, cdata_size);
  strncpy(node->value, cdata_text, cdata_size);
  node->namespace = NULL;
  node->attributes = NULL;
  node->children = NULL;
  return node;
}

dom_node* get_attribute_by_name(dom_node* node, char* attr_name){
  if(node->attributes == NULL)
    return NULL;

  return (dom_node*) search_rbtree(*(node->attributes), attr_name);
}

dom_node* get_child_at(dom_node* parent, int index){
  if(parent->children == NULL)
    return NULL;
  return (dom_node*) get_element_at(parent->children, index);
}

static void __get_elements_by_name(dom_node* root, char* name, list* lk){
  int it;

  if(root == NULL)
    return;

  if(root->type == ELEMENT && strcmp(root->name, name) == 0)
    append_element(lk, root, 0);

  if(root->children != NULL)
    for(it = 0; it < root->children->count; it++)
      __get_elements_by_name((dom_node*) get_element_at(root->children, it), name, lk);
}

struct generic_list_s* get_elements_by_name(doc* root, char* name){
  list* lk = new_generic_list(16);
  __get_elements_by_name(root->root, name, lk);
  return lk;
}

static void __get_text_nodes(dom_node* root, list* lk){
  int it;

  if(root == NULL)
    return;

  if(root->type == TEXT_NODE)
    append_element(lk, root, 0);

  if(root->children != NULL)
    for(it = 0; it < root->children->count; it++)
      __get_text_nodes((dom_node*) get_element_at(root->children, it), lk);
}

struct generic_list_s* get_text_nodes(doc* root){
  list* lk = new_generic_list(16);
  __get_text_nodes(root->root, lk);
  return lk;
}

list* get_children(dom_node* node){
  return node->children;
}

list* get_descendants(dom_node* node)
{
  if(node == NULL) return NULL;

  list* children = get_children(node);
  if(children == NULL) return NULL;

  list* descendants = new_generic_list(1);
  while(children->count > 0)
  {
    dom_node* n = pop_stack(children);
    if(n->type != ELEMENT) continue;

    list* nd = get_descendants(n);
    if(nd != NULL){
      descendants = merge_lists(descendants, nd);
    }
    add_element(descendants, n);
  }

  return descendants;
}

void destroy_dom_node(dom_node* n){
  struct siterator* ti;
  int it;

  if( n == NULL) return;
  if((n->type == ELEMENT || n->type == ATTRIBUTE) && n->namespace != NULL)
    free(n->namespace);
  if((n->type == ELEMENT || n->type == ATTRIBUTE) && n->name != NULL)
    free(n->name);
  if((n->type == TEXT_NODE || n->type == CDATA || n->type == ATTRIBUTE) && n->value != NULL)
    free(n->value);
  if(n->attributes != NULL){
    ti = new_tree_iterator(n->attributes);
    while(tree_iterator_has_next(ti))
      destroy_dom_node(tree_iterator_next(ti));
    destroy_iterator(ti);
    destroy_rbtree(n->attributes);
  }
  if(n->children != NULL){
    for(it = 0; it < n->children->count; it++)
      destroy_dom_node((dom_node*) get_element_at(n->children, it));
    destroy_generic_list(n->children);
  }
  free(n);
}

void destroy_dom_tree(doc* root){
  destroy_dom_node(root->root);
  free(root);
}

void delete_attribute(dom_node* node, char* name){
  rb_tree_delete(node->attributes, name);
}

void remove_node(doc* root, dom_node* node){
  if(node == root->root){
    root->root = NULL;
    return;
  }

  remove_element(node->parent->children, node);
  return;
}

