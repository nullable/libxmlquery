#include "serialize.h"
#include <string.h>

/*void main(){
    bson_buffer bb;
    bson b;
    bson_bool_t success;

    bson_buffer_init(&bb);
    bson_append_int(&bb, "integer", 1);
    bson_append_string(&bb, "string" , "foo:bar" );
    bson_from_buffer(&b, &bb);

    bson_print(&b);
}*/


byte_buffer* __node_list_to_xml(list* l, int depth);
byte_buffer* __node_list_to_json(list* l, int depth);

byte_buffer* __element_list_to_yaml(list* l, int depth);
byte_buffer* __text_list_to_yaml(list* l, int depth);

byte_buffer* __attribute_to_xml(dom_node* attr);
byte_buffer* __attribute_to_json(dom_node* attr, int depth);
byte_buffer* __attribute_to_yaml(dom_node* attr, int depth);


void destroy_byte_buffer(byte_buffer* bb){
  if(bb == NULL) return;

  free(bb->buffer);
  free(bb);
}

byte_buffer* new_byte_buffer(int initial){
    byte_buffer* b = alloc(byte_buffer, 1);
    b->buffer = alloc(char, 16);
    b->size = 0;
    b->capacity = initial;

    return b;
}

void append_string_to_buffer(const char* s, byte_buffer* b){
    int strl = strlen(s);
    append_bytes_to_buffer(s, b, strl);
}

void append_bytes_to_buffer(const char* bytes, byte_buffer* b, size_t size){
    if(b->size + size > b->capacity){
        b->buffer = (char*)realloc(b->buffer, sizeof(char)*(b->size+size)*2);
        b->capacity = (b->size+size)*2;
    }

    memcpy(b->buffer+b->size, bytes, size);
    b->size += size;
}

void append_buffer_to_buffer(byte_buffer* b2, byte_buffer* b){
    if(b->size + b2->size > b->capacity){
        b->buffer = (char*)realloc(b->buffer, (b->size+b2->size)*2);
        b->capacity = (b->size+b2->size)*2;
    }

    memcpy(b->buffer+b->size, b2->buffer, b2->size);
    b->size += b2->size;
}

byte_buffer* __dom_node_to_xml(dom_node* n, int depth){
    int i;
    byte_buffer* b = new_byte_buffer(16);
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
            }
            destroy_iterator(it);
        }
        if(n->children != NULL && n->children->count > 0){
            append_string_to_buffer(">\n", b);
            byte_buffer* children_xml = __node_list_to_xml(n->children, depth+1);
            append_buffer_to_buffer(children_xml, b);

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
    //append_string_to_buffer("{ ", b);
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
    //append_string_to_buffer("{ ", b);
    //append_string_to_buffer("- ", b);
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
        }
        destroy_iterator(it);
    }

    if(n->children != NULL && n->children->count > 0){
        for(i = 0; i < depth+1; i++){ append_string_to_buffer("  ", b); }
        append_string_to_buffer("- !children\n", b);
        byte_buffer* children_xml = __element_list_to_yaml(n->children, depth+2);
        append_buffer_to_buffer(children_xml, b);
    }

    if(n->children != NULL && n->children->count > 0){
        for(i = 0; i < depth+1; i++){ append_string_to_buffer("  ", b); }
        append_string_to_buffer("- !text >\n", b);
        byte_buffer* children_xml = __text_list_to_yaml(n->children, depth+2);
        append_buffer_to_buffer(children_xml, b);
    }

    if(n->namespace != NULL){
        for(i = 0; i < depth+1; i++){ append_string_to_buffer("  ", b); }
        append_string_to_buffer("- !ns ", b);
        append_string_to_buffer(n->namespace, b);
        append_string_to_buffer("\n", b);
    }

    //for(i = 0; i < depth; i++){ append_string_to_buffer("  ", b); }
    //append_string_to_buffer("},\n", b);

    return b;
}

byte_buffer* __dom_node_to_json(dom_node* n, int depth){
    int i;
    byte_buffer* b = new_byte_buffer(16);
    byte_buffer* node_json;
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
//char* __attribute_to_yaml(dom_node* n, char* buffer);


//char* __dom_element_to_bson(dom_node* n, char* buffer, int depth);
//char* __node_list_to_bson(list* l, char* buffer, int depth);
//char* __attribute_to_bson(dom_node* n, char* buffer);

char* document_to_string(doc* root, serialization_type t){
    byte_buffer* b;
    char* buff;
    switch(t){
    case XML:
        b = __dom_node_to_xml(root->root, 0);
        break;
    case JSON:
        b = __dom_node_to_json(root->root, 0);
        break;
    case YAML:
        b = __dom_node_to_yaml(root->root, 0);
        break;
    }
    append_bytes_to_buffer("\0", b, 1);
    buff = b->buffer;
    free(b);
    return buff;
}

