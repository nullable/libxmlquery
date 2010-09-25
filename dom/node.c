#include <stdio.h>
#include <regex.h>
#include <string.h>
#include "macros.h"
#include "node.h"

void set_namespace(dom_node* node, char* namespace){
  int size = strlen(namespace) + 1;

  if(node->namespace != NULL){
    free(node->namespace);
  }

  node->namespace = alloc(char, size);
  strncpy(node->namespace, namespace, size);
}

void set_name(dom_node* node, char* name){
  int size = strlen(name) + 1;

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

void set_xml_version(doc* document, char* vers){
  int size = strlen(vers) + 1;

  if(document->xml_version != NULL){
    free(document->xml_version);
  }

  document->xml_version = alloc(char, size);
  strncpy(document->xml_version, vers, size);
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
  if(child->type == attribute){
    log(W, "Trying to prepend attribute %s as child of node %s.\n", child->name, parent->name);
    return;
  }
  prepend(parent->children, child);
}

void append_child(dom_node* parent, dom_node* child){
  if(child->type == attribute){
    log(W, "Trying to append attribute %s as child of node %s.\n", child->name, parent->name);
    return;
  }
  if(node->children == NULL)
    node->children = new_list();
  append(parent->children, child);
}

void add_attribute(dom_node* node, dom_node* attribute){
  if(node->type == attribute){
    if(node->attributes == NULL)
      node->attributes = new_tree();
    red_black_tree_insert(node->attributes, node);
  }
  log(W, "Trying to add node %s as attribute of node %s\n", attribute->name, node->name);
  return;
}

doc* new_document(char* xml_version){
  doc* document = alloc(doc, 1);
  int vers_size = strlen(xml_version) + 1;
  doc->xml_version = alloc(char, vers_size);
  strncpy(doc->xml_version, xml_version, vers_size);
  doc->root = NULL;
  return doc;
}

dom_node* new_element_node(char* name){
  dom_node* node = alloc(dom_node, 1);
  int name_size = strlen(name) + 1;

  node->type = element;
  node->name = alloc(char, name_size);
  strncpy(node->name, name);
  node->value = NULL;
  node->namespace = NULL;
  node->attributes = NULL;
  node->children = NULL;
  return node;
}

dom_node* new_text_node(char* text){
  dom_node* node = alloc(dom_node, 1);
  int text_size = strlen(text) + 1;

  node->type = text;
  node->name = "~#TEXT#~";
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

  node->type = attribute;
  node->name = alloc(char,text_size);
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
  int cdata_size = strlen(cdata) + 1;

  node->type = cdata;
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

  return search(node->attributes, attr_name);
}

dom_node* get_child_at(dom_node* parent, int index){
  if(parent->children == NULL)
    return NULL;
  return get(parent->children, index);
}

list_keeper* regex_get_attributes(dom_node* node, char* pattern){
  if(node->attributes == NULL)
    return NULL;
  return regex_search(node->attributes, pattern);
}

list_keeper* regex_get_attributes_ignore_case(dom_node* node, char* pattern){
  if(node->attributes == NULL)
    return NULL;
  return regex_search_ignore_case(node->attributes, pattern);
}

static void __regex_get_elements_by_name(dom_node* root, char* pattern, list_keeper* lk, int ignore_case){
  list_node* it;

  if(root == NULL)
    return;

  if(root->type == element && match(root->name, pattern, ignore_case))
    append(lk, root);

  for(it = root->children->first; it != NULL; it = it->next)
    __regex_get_elements_by_name(it,pattern, lk, ignore_case);
}

list_keeper* regex_get_elements_by_name(doc* root, char* pattern){
  list_keeper* lk = new_list();
  __regex_get_elements_by_name(root->root, pattern, lk, 0);
  return lk;
}

list_keeper* regex_get_elements_by_name_ignore_case(doc* root, char* pattern){
  list_keeper* lk = new_list();
  __regex_get_elements_by_name(root->root, pattern, lk, 1);
  return lk;
}

static void __regex_get_elements_by_namespace(dom_node* root, char* pattern, list_keeper* lk, int ignore_case){
  list_node* it;

  if(root == NULL)
    return;

  if(root->type == element && match(root->namespace, pattern, ignore_case))
    append(lk, root);

  for(it = root->children->first; it != NULL; it = it->next)
    __regex_get_elements_by_namespace(it,pattern, lk, ignore_case);
}

list_keeper* regex_get_elements_by_namespace(doc* root, char* pattern){
  list_keeper* lk = new_list();
  __regex_get_elements_by_namespace(root->root, pattern, lk, 0);
  return lk;
}

list_keeper* regex_get_elements_by_namespace_ignore_case(doc* root, char* pattern){
  list_keeper* lk = new_list();
  __regex_get_elements_by_namespace(root->root, pattern, lk, 1);
  return lk;
}

list_keeper* get_children(dom_node* node){
  return node->children;
}

