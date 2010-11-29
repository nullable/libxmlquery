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
 
#include <string.h>
#include "../include/byte_buffer.h"
#include "../include/serialize.h"
#include "../include/rbtree.h"

byte_buffer* __node_list_to_xml(list* l, int depth);
byte_buffer* __node_list_to_json(list* l, int depth);

byte_buffer* __element_list_to_yaml(list* l, int depth);
byte_buffer* __text_list_to_yaml(list* l, int depth);

byte_buffer* __attribute_to_xml(dom_node* attr);
byte_buffer* __attribute_to_json(dom_node* attr, int depth);
byte_buffer* __attribute_to_yaml(dom_node* attr, int depth);

byte_buffer* __dom_node_to_xml(dom_node* n, int depth){
  int i;
  byte_buffer* b = new_byte_buffer(16);

  if(!n)
    return b;

  switch(n->type){
  case CDATA:
    for(i = 0; i < depth; i++){ append_string_to_buffer("  ", b); }
    append_string_to_buffer("<![CDATA[", b);
    append_string_to_buffer(n->value, b);
    append_string_to_buffer("]]>\n", b);
    break;
  case TEXT_NODE:
    for(i = 0; i < depth; i++){ append_string_to_buffer("  ", b); }
    append_string_to_buffer(n->value, b);
    append_string_to_buffer("\n", b);
    break;
  case ELEMENT:
    for(i = 0; i < depth; i++){ append_string_to_buffer("  ", b); }
    append_string_to_buffer("<", b);
    if(n->namespace != NULL)
      { append_string_to_buffer(n->namespace, b); append_string_to_buffer(":", b); }

    append_string_to_buffer(n->name, b);
    if(n->attributes != NULL){
      struct siterator *it = new_tree_iterator(n->attributes);
      while(tree_iterator_has_next(it)){
	byte_buffer* attr = __attribute_to_xml((dom_node*) tree_iterator_next(it));
	append_buffer_to_buffer(attr, b);
	destroy_byte_buffer(attr);
      }
      destroy_iterator(it);
    }
    if(n->children != NULL && n->children->count > 0){
      append_string_to_buffer(">\n", b);
      byte_buffer* children_xml = __node_list_to_xml(n->children, depth+1);
      append_buffer_to_buffer(children_xml, b);
      destroy_byte_buffer(children_xml);

      for(i = 0; i < depth; i++){ append_string_to_buffer("  ", b); }
      append_string_to_buffer("</", b);
      if(n->namespace != NULL){ append_string_to_buffer(n->namespace, b); append_string_to_buffer(":", b); }
      append_string_to_buffer(n->name, b);
      append_string_to_buffer(">\n", b);
    }
    else{
      append_string_to_buffer(" />\n", b);
    }
    break;
  case ATTRIBUTE:
    break;
  }
  return b;
}

byte_buffer* __dom_element_to_json(dom_node* n, int depth){
  byte_buffer* b = new_byte_buffer(16);
  int i;

  for(i = 0; i < depth; i++){ append_string_to_buffer("  ", b); }
  append_string_to_buffer(n->name, b);


  if(n->attributes == NULL && n->children == NULL && n->namespace == NULL){
    append_string_to_buffer(": null,\n", b);
    return b;
  }
  else{
    append_string_to_buffer(": {\n", b);
  }

  if(n->attributes != NULL){
    struct siterator *it = new_tree_iterator(n->attributes);
    while(tree_iterator_has_next(it)){
      byte_buffer* attr = __attribute_to_json((dom_node*) tree_iterator_next(it), depth+1);
      append_buffer_to_buffer(attr, b);
      destroy_byte_buffer(attr);
    }
    destroy_iterator(it);
  }

  if(n->children != NULL && n->children->count > 0){
    byte_buffer* children_xml = __node_list_to_json(n->children, depth+1);
    append_buffer_to_buffer(children_xml, b);
    destroy_byte_buffer(children_xml);
  }

  if(n->namespace != NULL){
    for(i = 0; i < depth+1; i++){ append_string_to_buffer("  ", b); }
    append_string_to_buffer("#ns: \"", b);
    append_string_to_buffer(n->namespace, b);
    append_string_to_buffer("\",\n", b);
  }

  for(i = 0; i < depth; i++){ append_string_to_buffer("  ", b); }
  append_string_to_buffer("},\n", b);

  return b;
}

byte_buffer* __dom_element_to_yaml(dom_node* n, int depth){
  byte_buffer* b = new_byte_buffer(16);
  int i;

  for(i = 0; i < depth; i++){ append_string_to_buffer("  ", b); }
  append_string_to_buffer(n->name, b);


  if(n->attributes == NULL && n->children == NULL && n->namespace == NULL){
    append_string_to_buffer(": null\n", b);
    return b;
  }
  else{
    append_string_to_buffer(":\n", b);
  }

  if(n->attributes != NULL){
    for(i = 0; i < depth+1; i++){ append_string_to_buffer("  ", b); }
    append_string_to_buffer("- !attributes\n", b);
    struct siterator *it = new_tree_iterator(n->attributes);
    i = 0;
    while(tree_iterator_has_next(it)){
      byte_buffer* attr = __attribute_to_yaml((dom_node*) tree_iterator_next(it), depth+2);
      append_buffer_to_buffer(attr, b);
      destroy_byte_buffer(attr);
    }
    destroy_iterator(it);
  }

  if(n->children != NULL && n->children->count > 0){
    for(i = 0; i < depth+1; i++){ append_string_to_buffer("  ", b); }
    append_string_to_buffer("- !children\n", b);
    byte_buffer* children_xml = __element_list_to_yaml(n->children, depth+2);
    append_buffer_to_buffer(children_xml, b);
    destroy_byte_buffer(children_xml);
  }

  if(n->children != NULL && n->children->count > 0){
    for(i = 0; i < depth+1; i++){ append_string_to_buffer("  ", b); }
    append_string_to_buffer("- !text >\n", b);
    byte_buffer* children_xml = __text_list_to_yaml(n->children, depth+2);
    append_buffer_to_buffer(children_xml, b);
    destroy_byte_buffer(children_xml);
  }

  if(n->namespace != NULL){
    for(i = 0; i < depth+1; i++){ append_string_to_buffer("  ", b); }
    append_string_to_buffer("- !ns ", b);
    append_string_to_buffer(n->namespace, b);
    append_string_to_buffer("\n", b);
  }

  return b;
}

byte_buffer* __dom_node_to_json(dom_node* n, int depth){
  int i;
  byte_buffer* b = new_byte_buffer(16);
  byte_buffer* node_json;

  if(!n)
    return b;

  switch(n->type){
  case CDATA:
  case TEXT_NODE:
    for(i = 0; i < depth; i++){ append_string_to_buffer("  ", b); }
    append_string_to_buffer("\"#text\": \"", b);
    append_string_to_buffer(n->value, b);
    append_string_to_buffer("\",\n", b);
    break;
  case ELEMENT:
    node_json = __dom_element_to_json(n, depth);
    append_buffer_to_buffer(node_json, b);
    destroy_byte_buffer(node_json);
    break;
  case ATTRIBUTE:
    break;
  }
  return b;
}

byte_buffer* __dom_node_to_yaml(dom_node* n, int depth){
  int i;
  byte_buffer* b = new_byte_buffer(16);

  if(!n)
    return b;

  byte_buffer* node_yaml;
  switch(n->type){
  case CDATA:
  case TEXT_NODE:
    for(i = 0; i < depth; i++){ append_string_to_buffer("  ", b); }
    append_string_to_buffer(n->value, b);
    append_string_to_buffer("\n", b);
    break;
  case ELEMENT:
    node_yaml = __dom_element_to_yaml(n, depth);
    append_buffer_to_buffer(node_yaml, b);
    destroy_byte_buffer(node_yaml);
    break;
  case ATTRIBUTE:
    break;
  }
  return b;
}

byte_buffer* __node_list_to_xml(list* l, int depth){
  byte_buffer* b = new_byte_buffer(16);
  byte_buffer* element_buffer;
  int i;
  for(i = 0; i < l->count; i++){
    element_buffer = __dom_node_to_xml(get_element_at(l, i), depth);
    append_buffer_to_buffer(element_buffer, b);
    destroy_byte_buffer(element_buffer);
  }
  return b;
}

byte_buffer* __node_list_to_json(list* l, int depth){
  byte_buffer* b = new_byte_buffer(16);
  byte_buffer* element_buffer;
  int i;
  for(i = 0; i < l->count; i++){
    element_buffer = __dom_node_to_json(get_element_at(l, i), depth);
    append_buffer_to_buffer(element_buffer, b);
    destroy_byte_buffer(element_buffer);
  }
  return b;
}

byte_buffer* __element_list_to_yaml(list* l, int depth){
  byte_buffer* b = new_byte_buffer(16);
  int i;
  byte_buffer* element_buffer;
  for(i = 0; i < l->count; i++){
    if(((dom_node*)get_element_at(l, i))->type == ELEMENT){
      element_buffer = __dom_node_to_yaml(get_element_at(l, i), depth);
      append_buffer_to_buffer(element_buffer, b);
      destroy_byte_buffer(element_buffer);
    }
  }
  return b;
}

byte_buffer* __text_list_to_yaml(list* l, int depth){
  byte_buffer* b = new_byte_buffer(16);
  int i;
  byte_buffer* element_buffer;
  for(i = 0; i < l->count; i++){
    if(((dom_node*)get_element_at(l, i))->type == TEXT_NODE){
      element_buffer = __dom_node_to_yaml(get_element_at(l, i), depth);
      append_buffer_to_buffer(element_buffer, b);
      destroy_byte_buffer(element_buffer);
    }
  }
  return b;
}
//char* __node_list_to_yaml(list* l, char* buffer, int depth);

byte_buffer* __attribute_to_xml(dom_node* attr){
  byte_buffer* b = new_byte_buffer(16);

  append_string_to_buffer(" ", b);
  if(attr->namespace != NULL){
    append_string_to_buffer(attr->namespace, b);
    append_string_to_buffer(":", b);
  }
  append_string_to_buffer(attr->name, b);
  append_string_to_buffer("=\"", b);
  append_string_to_buffer(attr->value, b);
  append_string_to_buffer("\"", b);
  return b;
}

byte_buffer* __attribute_to_json(dom_node* attr, int depth){
  byte_buffer* b = new_byte_buffer(16);
  int i;

  for(i = 0; i < depth; i++){
    append_string_to_buffer("  ", b);
  }
  append_string_to_buffer(attr->name, b);
  append_string_to_buffer(": \"", b);
  append_string_to_buffer(attr->value, b);
  append_string_to_buffer("\",\n", b);
  return b;
}

byte_buffer* __attribute_to_yaml(dom_node* attr, int depth){
  byte_buffer* b = new_byte_buffer(16);
  int i;

  for(i = 0; i < depth; i++){ append_string_to_buffer("  ", b); }
  append_string_to_buffer(attr->name, b);
  append_string_to_buffer(": \"", b);
  append_string_to_buffer(attr->value, b);
  append_string_to_buffer("\"\n", b);
  return b;
}

char* node_to_string(dom_node* root, serialization_type t){
  byte_buffer* b;
  char* buff;
  switch(t){
  case XML:
    b = __dom_node_to_xml(root, 0);
    break;
  case JSON:
    b = __dom_node_to_json(root, 0);
    break;
  case YAML:
    b = __dom_node_to_yaml(root, 0);
    break;
  }
  append_bytes_to_buffer("\0", b, 1);
  buff = b->buffer;
  free(b);
  return buff;
}

