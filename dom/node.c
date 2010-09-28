#include <stdio.h>
#include <regex.h>
#include <string.h>
#include "macros.h"
#include "node.h"
#include "eregex.h"

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
  if(child->type == ATTRIBUTE){
    log(W, "Trying to prepend attribute %s as child of node %s.\n", child->name, parent->name);
    return;
  }
  if(parent->children == NULL)
    parent->children = new_list();
  prepend(parent->children, child);
}

void append_child(dom_node* parent, dom_node* child){
  if(child->type == ATTRIBUTE){
    log(W, "Trying to append attribute %s as child of node %s.\n", child->name, parent->name);
    return;
  }
  if(parent->children == NULL)
    parent->children = new_list();
  append(parent->children, child);
}

void add_attribute(dom_node* node, dom_node* attribute){
  if(attribute->type == ATTRIBUTE){
    if(node->attributes == NULL)
      node->attributes = new_tree();
    red_black_tree_insert(node->attributes, attribute);
    return;
  }
  log(W, "Trying to add node %s as attribute of node %s\n", attribute->name, node->name);
  return;
}

void append_children(dom_node* parent, struct slist_keeper* children){
  if(parent->children == NULL)
    parent->children = new_list();
  add_all(parent->children, children);
  destroy(children);
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

  return search(*(node->attributes), attr_name);
}

dom_node* get_child_at(dom_node* parent, int index){
  if(parent->children == NULL)
    return NULL;
  return get(*(parent->children), index);
}

static void __get_elements_by_name(dom_node* root, char* name, list_keeper* lk){
  list_node* it;

  if(root == NULL)
    return;

  if(root->type == ELEMENT && strcmp(root->name, name) == 0)
    append(lk, root);

  if(root->children != NULL)
    for(it = root->children->first; it != NULL; it = it->next)
      __get_elements_by_name(it->node, name, lk);
}

struct slist_keeper* get_elements_by_name(doc* root, char* name){
  list_keeper* lk = new_list();
  __get_elements_by_name(root->root, name, lk);
  return lk;
}

static void __get_text_nodes(dom_node* root, list_keeper* lk){
  list_node* it;

  if(root == NULL)
    return;

  if(root->type == TEXT_NODE)
    append(lk, root);

  if(root->children != NULL)
    for(it = root->children->first; it != NULL; it = it->next)
      __get_text_nodes(it->node, lk);
}

struct slist_keeper* get_text_nodes(doc* root){
  list_keeper* lk = new_list();
  __get_text_nodes(root->root, lk);
  return lk;
}

list_keeper* regex_get_attributes(dom_node* node, char* pattern){
  if(node->attributes == NULL)
    return NULL;
  return regex_search(*(node->attributes), pattern);
}

list_keeper* regex_get_attributes_ignore_case(dom_node* node, char* pattern){
  if(node->attributes == NULL)
    return NULL;
  return regex_search_ignore_case(*(node->attributes), pattern);
}

static void __regex_get_elements_by_name(dom_node* root, char* pattern, list_keeper* lk, int ignore_case){
  list_node* it;

  if(root == NULL)
    return;

  if(root->type == ELEMENT && match(root->name, pattern, ignore_case))
    append(lk, root);

  if(root->children != NULL)
    for(it = root->children->first; it != NULL; it = it->next)
      __regex_get_elements_by_name(it->node, pattern, lk, ignore_case);
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

  if(root->type == ELEMENT && match(root->namespace, pattern, ignore_case))
    append(lk, root);
  
  if(root->children != NULL)
    for(it = root->children->first; it != NULL; it = it->next)
      __regex_get_elements_by_namespace(it->node, pattern, lk, ignore_case);
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

void destroy_dom_node(dom_node* n){
  struct siterator* ti;
  struct slist_iterator* it;
  if(n->namespace != NULL) free(n->namespace);
  if(n->name != NULL) free(n->name);
  if(n->value != NULL) free(n->value);
  if(n->attributes != NULL){
    ti = new_tree_iterator(n->attributes);
    while(tree_iterator_has_next(ti))
      destroy_dom_node(tree_iterator_next(ti));
    destroy_iterator(ti);
    destroy_tree(n->attributes);
  }
  if(n->children != NULL){
    it = new_list_iterator(n->children);
    while(list_iterator_has_next(it))
      destroy_dom_node(list_iterator_next(it));
    destroy_list_iterator(it);
    destroy(n->children);
  }
  free(n);
}

void destroy_dom_tree(doc* root){
  destroy_dom_node(root->root);
  free(root);
}

static void __output_xml(dom_node* root, int pad){
  int i;
  list_node* it;
  
  if(root == NULL)
    return;

  switch(root->type){
  case ELEMENT:
    {
      for(i = 0; i < pad; i++, printf(" "));
  
      printf("<");
      if(root->namespace != NULL)
	printf("%s:", root->namespace);
      printf("%s ", root->name);

      if(root->attributes != NULL){
	struct siterator *it = new_tree_iterator(root->attributes);
	while(tree_iterator_has_next(it)){
	  dom_node* attr = tree_iterator_next(it);
	  printf("%s=\"", attr->name);
	  if(attr->namespace != NULL)
	    printf("%s:", attr->namespace);
	  printf("%s\" ", attr->value);
	}
      }
  
      printf(">\n");     
      break;
    }
  case CDATA:
    {
      for(i = 0; i < pad; i++, printf(" "));
      printf("<![CDATA[");
      /*no break; Will fall to case TEXT_NODE:*/
    }
  case TEXT_NODE:
    {
      printf("%s", root->value);
      break;
    }
  case ATTRIBUTE: break;
  default:
    log(W, "xml_output found an inconsistency in the DOM tree.\n");
  }

  if(root->children != NULL)
    for(it = root->children->first; it != NULL; it = it->next)
      __output_xml(it->node, pad + 1);

  switch(root->type){
  case ELEMENT:
    {
      for(i = 0; i < pad; i++, printf(" "));
      printf("</");
      if(root->namespace != NULL)
	printf("%s:", root->namespace);
      printf("%s>\n", root->name);
      break;
    }
  case CDATA:
    {
      for(i = 0; i < pad; i++, printf(" "));
      printf("]]>\n");
      break;
    }
  case ATTRIBUTE: break;
  case TEXT_NODE: break;
  }
}

void output_xml(doc* root){
  if(root->xml_declaration != NULL){
    printf("<?%s ", root->xml_declaration->name);
    struct siterator *it = new_tree_iterator(root->xml_declaration->attributes);
    while(tree_iterator_has_next(it)){
      dom_node* attr = tree_iterator_next(it);
      printf("%s=\"", attr->name);
      if(attr->namespace != NULL)
	printf("%s:", attr->namespace);
      printf("%s\" ", attr->value);
    }
    printf("?>\n");
  }
  __output_xml(root->root, 0);
  fflush(stdout);
}
