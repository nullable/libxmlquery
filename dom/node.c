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

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../include/macros.h"
#include "../include/node.h"

#define USE_DICT 1



struct sroot* dictionary = NULL;

void initialize_dictionary(){
    dictionary = new_rbtree(&ident, &compare_string);
}

char* get_string_pointer(const char* string){
    char* value = NULL;
    if(string == NULL){ return NULL; }
    if(USE_DICT){
        if(!dictionary){ initialize_dictionary(); }
        value = search_rbtree(dictionary, (const void*)string);
        if(value == NULL){
            value = strdup(string);
            rb_tree_insert(dictionary, value);
        }
    }
    else{
        value = strdup(string);
    }

    //log(I, "%p: '%s' | '%s'", value, value, string);
    return value;
}

char* set_namespace(dom_node* node, const char* namespace){
  char* old;

  if(!namespace)
    return NULL;
  old = node->namespace;
  node->namespace = get_string_pointer(namespace);

  return old;
}

char* set_name(dom_node* node, char* name){
  char* old;

  if(!name)
    return NULL;

  old = node->name;
  node->name = get_string_pointer(name);
  return (USE_DICT)? NULL: old;
}

char* set_value(dom_node* node, char* value){
  char* old;

  if(!value)
    return NULL;

  if(node->type == ELEMENT){ log(F, "Element node does not support the value attribute."); exit(1); }

  old = node->value;
  node->value = get_string_pointer(value);
  return (USE_DICT)? NULL: old;
}

dom_node* set_doc_root(doc* document, struct snode* root){
  dom_node* old;
  if(!document)
    return NULL;
  old = document->root;
  document->root = root;
  return old;
}

list* set_xml_declaration(doc* document, list* declarations){
  list* old;
  if(!document)
    return NULL;
  old = document->xml_declarations;
  document->xml_declarations = declarations;
  return old;
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

list* get_xml_declarations(doc* document){
  return document->xml_declarations;
}

void prepend_child(dom_node* parent, dom_node* child){
  if(child == NULL) {
    log(W, "Trying to prepend a NULL child.");
    return;
  }
  if(child->type == ATTRIBUTE){
    log(W, "Trying to prepend attribute %s as child of node %s.", child->name, parent->name);
    return;
  }
  if(parent->children == NULL)
    parent->children = new_generic_list(1);

  //log(W, "prepend_child needs to be implemented.");

  prepend_element_with_type(parent->children, child, 0);
  child->parent = parent;
}

void append_child(dom_node* parent, dom_node* child){
  if(child == NULL) {
    log(W, "Trying to append a NULL child.");
    return;
  }
  if(child->type == ATTRIBUTE){
    log(W, "Trying to append attribute %s as child of node %s.", child->name, parent->name);
    return;
  }
  if(child->parent != NULL){
    remove_element(child->parent->children, child);
  }

  if(parent->children == NULL)
    parent->children = new_generic_list(1);
  add_element(parent->children, child);
  child->parent = parent;
}

const void* dom_node_key(const struct stree_node* node){
  return ((dom_node*) node->node)->name;
}

void add_attribute(dom_node* node, dom_node* attribute){
  if(attribute == NULL) {
    log(W, "Trying to add a NULL attrubute.");
    return;
  }
  if(attribute->type == ATTRIBUTE){
    if(node->attributes == NULL)
      node->attributes = new_rbtree(&dom_node_key, &compare_string);
    dom_node* older = (dom_node*) rb_tree_insert(node->attributes, attribute);
    if(older != NULL)
      destroy_dom_node(older);
    return;
  }
  log(W, "Trying to add node %s as attribute of node %s", attribute->name, node->name);
  return;
}

void append_children(dom_node* parent, struct generic_list_s* children){
  int i;
  if(children == NULL){
    log(W, "Trying to append NULL children.");
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

doc* new_document(list* xml_declarations){
  doc* document = alloc(doc, 1);
  set_xml_declaration(document, xml_declarations);
  return document;
}

dom_node* new_element_node(const char* name){
  dom_node* node = alloc(dom_node, 1);

  node->value = NULL;
  node->namespace = NULL;
  node->attributes = NULL;
  node->children = NULL;
  node->parent = NULL;

  node->type = ELEMENT;
  node->name = get_string_pointer(name);
  return node;
}

dom_node* new_text_node(char* text){
  //dangerous memory optimization
  textnode_dom_node* node = alloc(textnode_dom_node, 1);

  //node->namespace = NULL;
  //node->attributes = NULL;
  //node->children = NULL;
  //node->name = NULL;

  node->parent = NULL;
  node->type = TEXT_NODE;
  node->value = get_string_pointer(text);
  return (dom_node*)node;
}

dom_node* new_attribute(char* name, char* value){
  dom_node* node = alloc(dom_node, 1);

  node->namespace = NULL;
  node->attributes = NULL;
  node->children = NULL;

  node->type = ATTRIBUTE;
  node->name = get_string_pointer(name);
  node->value = get_string_pointer(value);
  return node;
}

dom_node* new_cdata(char* cdata_text){
  dom_node* node = alloc(dom_node, 1);

  node->name = NULL;
  node->namespace = NULL;
  node->attributes = NULL;
  node->children = NULL;

  node->type = CDATA;
  node->value = get_string_pointer(cdata_text);
  return node;
}

dom_node* get_attribute_by_name(dom_node* node, char* attr_name){
  if(node->attributes == NULL)
    return NULL;

  return (dom_node*) search_rbtree(node->attributes, attr_name);
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
    append_element_with_type(lk, root, 0);

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
    append_element_with_type(lk, root, 0);

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
  int i;
  if(node == NULL) return NULL;

  list* children = get_children(node);
  if(children == NULL) return NULL;

  list* descendants = new_generic_list(1);
  for(i = 0; i < children->count; i++)
  {
    dom_node* n = get_element_at(children, i);
    if(n->type != ELEMENT) continue;

    list* nd = get_descendants(n);
    descendants = merge_lists(descendants, nd);
    add_element(descendants, n);
  }

  return descendants;
}

void destroy_dom_node(dom_node* n){
  struct siterator* ti;
  int it;

  if( n == NULL) return;
  if((n->type == ELEMENT || n->type == ATTRIBUTE) && n->namespace != NULL)
    if(!USE_DICT) free(n->namespace);
  if((n->type == ELEMENT || n->type == ATTRIBUTE) && n->name != NULL)
    if(!USE_DICT) free(n->name);
  if((n->type == TEXT_NODE || n->type == CDATA || n->type == ATTRIBUTE) && n->value != NULL)
    if(!USE_DICT) free(n->value);
  if(n->type == ELEMENT && n->attributes != NULL){
    ti = new_tree_iterator(n->attributes);
    while(tree_iterator_has_next(ti))
      destroy_dom_node(tree_iterator_next(ti));
    destroy_iterator(ti);
    destroy_rbtree(n->attributes);
  }
  if(n->type == ELEMENT && n->children != NULL){
    for(it = 0; it < n->children->count; it++)
      destroy_dom_node((dom_node*) get_element_at(n->children, it));
    destroy_generic_list(n->children);
  }
  free(n);
}

void destroy_dictionary(){
    if(dictionary != NULL){
        struct siterator* ti = new_tree_iterator(dictionary);
        while(tree_iterator_has_next(ti))
            free(tree_iterator_next(ti));
        destroy_iterator(ti);
        destroy_rbtree(dictionary);
        dictionary = NULL;
    }
}

void destroy_dom_tree(doc* root){
  destroy_dom_node(root->root);
  if(root->xml_declarations){
      int i;
      for(i = 0; i < root->xml_declarations->count; i++){
          destroy_dom_node(get_element_at(root->xml_declarations, i));
      }
      destroy_generic_list(root->xml_declarations);
  }

  free(root);
}

dom_node* delete_attribute(dom_node* node, char* name){
  return (dom_node*) rb_tree_delete(node->attributes, name);
}

void remove_node(doc* root, dom_node* node){
  if(node == root->root){
    root->root = NULL;
    return;
  }

  remove_element(node->parent->children, node);
  return;
}

