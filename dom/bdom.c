#include <string.h>
#include <stdint.h>
#include "../include/macros.h"
#include "../include/bdom.h"

int* get_size_pointer(bdom* bd){
  return (int*) (bd->bb->buffer);
}

uint8_t* get_type_pointer(bdom* bd){
  return (uint8_t *) (bd->bb->buffer + sizeof(int));
}

bdom* init_bdom(enum bdom_type t){
  bdom* b = alloc(bdom, 1); 

  b->bb = new_byte_buffer(sizeof(int) + 1);
  *(get_size_pointer(b)) = 0;
  *(get_type_pointer(b)) = t;

  /*  char zero_int[sizeof(int)] = {0};
  append_bytes_to_buffer(zero_int, b->bb, sizeof(int));
  
  char t_int[sizeof(int)];
  memcpy(t_int, &t, sizeof(int));
  append_bytes_to_buffer(t_int, b->bb, sizeof(int));*/

  b->bb->size = b->bb->capacity;

  return b;
}

void destroy_bdom(bdom* b){
  if(!b) return;

  destroy_byte_buffer(b->bb);
  free(b);
}

/*Append bdom "from" to bdom "to".*/
void append_bdom_to_bdom(bdom* from, bdom* to){
  *(get_size_pointer(to)) += *(get_size_pointer(from));
  append_buffer_to_buffer(from->bb, to->bb);
}

bdom* bdom_from_string(const char* str, enum bdom_type t){
  bdom* b = init_bdom(t);

  append_string_to_buffer(str, b->bb);
  *(get_size_pointer(b)) = strlen(str);

  return b;
}

bdom* bdom_from_dom_node(dom_node* n){
  bdom* b;

  switch(n->type){
  case ELEMENT:
    b = init_bdom(BDOM_NODE);
    break;
  case ATTRIBUTE:
    b = init_bdom(BDOM_ATTR);
    break;
  case TEXT_NODE:
    b = init_bdom(BDOM_TEXT);
    break;
  case CDATA:
    b = init_bdom(BDOM_CDATA);
    break;
  }

  if((n->type == ELEMENT || n->type == ATTRIBUTE) && n->name != NULL){
    bdom *name = bdom_from_string(n->name, BDOM_NAME);
    append_bdom_to_bdom(name, b);
    destroy_bdom(name);
  }
  if((n->type == ELEMENT || n->type == ATTRIBUTE) && n->namespace != NULL){
    bdom *namespace = bdom_from_string(n->name, BDOM_NAMESPACE);
    append_bdom_to_bdom(namespace, b);
    destroy_bdom(namespace);
  }
  if((n->type == TEXT_NODE || n->type == CDATA || n->type == ATTRIBUTE) && n->value != NULL){
    bdom *value = bdom_from_string(n->value, BDOM_VAL);
    append_bdom_to_bdom(value, b);
    destroy_bdom(value);
  }

  if(n->attributes != NULL){    
    tree_iterator* it = new_tree_iterator(n->attributes);
    while(tree_iterator_has_next(it)){
      dom_node* attr = tree_iterator_next(it);
      bdom* battr = bdom_from_dom_node(attr);
      append_bdom_to_bdom(battr, b);
      destroy_bdom(battr);
    }
    destroy_iterator(it);
  }

  if(n->children != NULL){
    int i;
    for(i = 0; i < n->children->count; i++){
      bdom* child = bdom_from_dom_node(get_element_at(n->children,i));
      append_bdom_to_bdom(child, b);
      destroy_bdom(child);
    }
  }

  return b;
}
