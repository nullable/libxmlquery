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
        b->buffer = (char*)realloc(b->buffer, (b->size+size)*2);
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

byte_buffer* __dom_element_to_xml(dom_node* n, int depth){
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
//char* __dom_element_to_json(dom_node* n, char* buffer, int depth);
//char* __dom_element_to_yaml(dom_node* n, char* buffer, int depth);

byte_buffer* __node_list_to_xml(list* l, int depth){
    byte_buffer* b = new_byte_buffer(16);
    int i;
    for(i = 0; i < l->count; i++){
        byte_buffer* element_buffer = __dom_element_to_xml(get_element_at(l, i), depth);
        append_buffer_to_buffer(element_buffer, b);
        //TODO free buffer
    }
    return b;
}

//char* __node_list_to_json(list* l, char* buffer, int depth);
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

//char* __attribute_to_json(dom_node* n, char* buffer);
//char* __attribute_to_yaml(dom_node* n, char* buffer);


//char* __dom_element_to_bson(dom_node* n, char* buffer, int depth);
//char* __node_list_to_bson(list* l, char* buffer, int depth);
//char* __attribute_to_bson(dom_node* n, char* buffer);


char* to_xml(doc* root){
    byte_buffer* b = __dom_element_to_xml(root->root, 0);
    append_bytes_to_buffer("\0", b, 1);
    return b->buffer;
}

static void __output_xml(dom_node* root, int pad){
  int i, it;

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
	  dom_node* attr = (dom_node*) tree_iterator_next(it);
	  if(attr->namespace != NULL)
	    printf("%s:", attr->namespace);
	  printf("%s=\"", attr->name);
	  printf("%s\" ", attr->value);
	}
	destroy_iterator(it);
      }

      printf(">\n");
      break;
    }
  case CDATA:
    {
      for(i = 0; i < pad; i++, printf(" "));
      printf("<![CDATA[");
      //no break; Will fall to case TEXT_NODE:
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
    for(it = 0; it < root->children->count; it++)
      __output_xml((dom_node*) get_element_at(root->children, i), pad + 1);

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
      dom_node* attr = (dom_node*) tree_iterator_next(it);
      printf("%s=\"", attr->name);
      if(attr->namespace != NULL)
	  printf("%s:", attr->namespace);
      printf("%s\" ", attr->value);
    }
    destroy_iterator(it);
    printf("?>\n");
  }
  __output_xml(root->root, 0);
  fflush(stdout);
}

