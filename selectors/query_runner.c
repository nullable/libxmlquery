#include <string.h>

#include "../include/query_runner.h"
#include "../include/stack.h"
#include "../include/node.h"
#include "../include/lxq_parser.h"
#include "../include/y.tab.h"
#include "../include/query_parser.h"
#include "../include/eregex.h"
#include "../include/byte_buffer.h"

list* run_query(queue* query, const list* all_nodes);

list* filter_nodes_by_type(list* nodes, enum node_type type){
    if(nodes == NULL) return NULL;
    int i;
    list* r = new_generic_list(nodes->capacity);
    for(i = 0; i < nodes->count; i++){
        if( ((dom_node*)get_element_at(nodes, i))->type == type){
            add_element(r, get_element_at(nodes, i));
        }
    }

    return r;
}

list* get_xml_descendants(dom_node* n){
  list* nodes = get_descendants(n), *res;
  res = filter_nodes_by_type(nodes, ELEMENT);
  destroy_generic_list(nodes);
  return res;
}

list* get_xml_children(dom_node* n){
  list* nodes = get_children(n);
  return filter_nodes_by_type(nodes, ELEMENT);
}

list* get_xml_nodes_after(dom_node* n){
    list* r = new_generic_list(16);
    int i;

    if(n->parent == NULL){ return r; }
    list* siblings = get_children(n->parent);

    for(i = get_element_pos(siblings, n); i < siblings->count; i++){
        add_element(r, get_element_at(siblings, i));
    }

    return r;
}

dom_node* get_xml_node_after(dom_node* n){

    if(n->parent == NULL){ return NULL; }
    list* siblings = get_children(n->parent);

    int p = get_element_pos(siblings, n) + 1;
    if(siblings->count <= p) return NULL;
    else return get_element_at(siblings, p);
}


list* apply_operator(const list* nodes, int op){
    int i;
    dom_node* next_node;
    list* result = new_generic_list(1);
    tree_root* rbtree = new_simple_rbtree();

    for(i = 0; i < nodes->count; i++){
        dom_node* node = get_element_at(nodes, i);
        switch(op){
        case SPACE:
	  {
	  //result = merge_lists(result, get_xml_descendants(node));
	    list* desc = get_xml_descendants(node);
	    generic_list_iterator* it = new_generic_list_iterator(desc);
	    while(generic_list_iterator_has_next(it)){
	      void* aux = generic_list_iterator_next(it);
	      void* old = rb_tree_insert(rbtree, aux);
	      if(!old)
		add_element(result, aux);
	    }
	    destroy_generic_list_iterator(it);
	    destroy_generic_list(desc);
            break;
	  }
        case '>':
	  {
	    // result = merge_lists(result, get_xml_children(node));
	    list* c = get_xml_children(node);
	    generic_list_iterator* it = new_generic_list_iterator(c);
	    while(generic_list_iterator_has_next(it)){
	      void* aux = generic_list_iterator_next(it);
	      void* old = rb_tree_insert(rbtree, aux);
	      if(!old)
		add_element(result, aux);
	    }
	    destroy_generic_list_iterator(it);
	    destroy_generic_list(c);
            break;
	  }
        case '~':
	  {
	    // result = merge_lists(result, get_xml_nodes_after(node));
	    list* a = get_xml_nodes_after(node);
	    generic_list_iterator* it = new_generic_list_iterator(a);
	    while(generic_list_iterator_has_next(it)){
	      void* aux = generic_list_iterator_next(it);
	      void* old = rb_tree_insert(rbtree, aux);
	      if(!old)
		add_element(result, aux);
	    }
	    destroy_generic_list_iterator(it);
	    destroy_generic_list(a);
            break;
	  }
        case '+':
	  {
            next_node = get_xml_node_after(node);
	    // if(next_node != NULL) add_element(result, next_node);
	    if(next_node != NULL){
	      void* old = rb_tree_insert(rbtree, next_node);
	      if(!old)
		add_element(result, next_node);
	    }
            break;
	  }
        }
    }
    //    return remove_duplicates(result);
    destroy_rbtree(rbtree);
    return result;
}

list* filter_nodes_by_name(const list* nodes, char* name){
    int i;
    if(nodes == NULL) return NULL;
    if(nodes->count == 0) return NULL;

    list* r = new_generic_list(nodes->capacity);

    for(i = 0; i < nodes->count; i++){
        if(!strcmp(name, ((dom_node*)get_element_at(nodes, i))->name)){
            add_element(r, get_element_at(nodes, i));
        }
    }

    return r;
}

list* filter_nodes_by_attr(const list* nodes, attr_selector* attr_s){
    int i;
    byte_buffer* bb;
    if(nodes == NULL) return NULL;
    if(nodes->count == 0) return NULL;

    list* r = new_generic_list(nodes->capacity);

    for(i = 0; i < nodes->count; i++){
        dom_node* n = get_element_at(nodes, i);
        dom_node* attr = get_attribute_by_name(n, attr_s->name);
        if(attr == NULL || strcmp(attr->name, attr_s->name)) continue;
        switch(attr_s->op){
        case EQUAL_OP:
            if(!strcmp(attr->value, attr_s->value)){
                add_element(r, n);
            }
            break;
        case NOTEQUAL_OP:
            if(strcmp(attr->value, attr_s->value)){
                add_element(r, n);
            }
            break;
        case NO_OP:
	        add_element(r, n);
	        break;
        case CONTAINS_OP:
        case REGEX_OP:
            if(match(attr->value, attr_s->value, 0)) add_element(r, n);
            break;
        case REGEXI_OP:
            if(match(attr->value, attr_s->value, 1)) add_element(r, n);
            break;
        case STARTSW_OP:
            bb = new_byte_buffer(strlen(attr_s->value)+2);
            append_string_to_buffer("^", bb);
            append_string_to_buffer(attr_s->value, bb);
            append_bytes_to_buffer("\0", bb, 1);
            if(match(attr->value, bb->buffer, 0)) add_element(r, n);
            break;
        case ENDSW_OP:
            bb = new_byte_buffer(strlen(attr_s->value)+2);
            append_string_to_buffer(attr_s->value, bb);
            append_string_to_buffer("$", bb);
            append_bytes_to_buffer("\0", bb, 1);
            if(match(attr->value, bb->buffer, 0)) add_element(r, n);
            break;
        case WSSV_OP:
            bb = new_byte_buffer((strlen(attr_s->value)+5)*4);
            append_string_to_buffer("(^", bb);
            append_string_to_buffer(attr_s->value, bb);
            append_string_to_buffer("$)|", bb);

            append_string_to_buffer("(^", bb);
            append_string_to_buffer(attr_s->value, bb);
            append_string_to_buffer(" )|", bb);

            append_string_to_buffer("( ", bb);
            append_string_to_buffer(attr_s->value, bb);
            append_string_to_buffer("$)|", bb);

            append_string_to_buffer("( ", bb);
            append_string_to_buffer(attr_s->value, bb);
            append_string_to_buffer(" )", bb);

            append_bytes_to_buffer("\0", bb, 1);
            if(match(attr->value, bb->buffer, 0)) add_element(r, n);
            break;
        case DSV_OP:
            bb = new_byte_buffer((strlen(attr_s->value)+5)*4);
            append_string_to_buffer("(^", bb);
            append_string_to_buffer(attr_s->value, bb);
            append_string_to_buffer("$)|", bb);

            append_string_to_buffer("(^", bb);
            append_string_to_buffer(attr_s->value, bb);
            append_string_to_buffer("-)|", bb);

            append_string_to_buffer("(-", bb);
            append_string_to_buffer(attr_s->value, bb);
            append_string_to_buffer("$)|", bb);

            append_string_to_buffer("(-", bb);
            append_string_to_buffer(attr_s->value, bb);
            append_string_to_buffer("-)", bb);

            append_bytes_to_buffer("\0", bb, 1);
            if(match(attr->value, bb->buffer, 0)) add_element(r, n);
            break;
        default:
            log(F, "Special attribute operators not implemented.\n");
        }
    }

    return r;
}

list* filter_nodes_by_pseudo_filter(const list* nodes, filter_selector* filter_s, const list* all_nodes){
    int i, m, o, pos;
    dom_node* n;
    list* siblings = NULL;
    list* children;

    if(nodes == NULL) return NULL;
    if(nodes->count == 0) return NULL;

    list* r = new_generic_list(nodes->capacity);
    list* not_nodes;

    if(filter_s->op == NOT_FILTER) not_nodes = run_query(filter_s->value.selector, all_nodes);

    for(i = 0; i < nodes->count; i++){
        n = get_element_at(nodes, i);
        switch(filter_s->op){
        case FIRST_CHILD_FILTER:
        case LAST_CHILD_FILTER:
        case ONLY_CHILD_FILTER:
        case NTH_CHILD_FILTER:
        case NTH_LAST_CHILD_FILTER:
            if(n->parent == NULL) continue;
            siblings = filter_nodes_by_type(get_children(n->parent), ELEMENT);
        }

        switch(filter_s->op){
        case FIRST_CHILD_FILTER:
            if(get_element_pos(siblings, n) == 0)
                add_element(r, n);
            break;
        case LAST_CHILD_FILTER:
            if(get_element_pos(siblings, n) == siblings->count-1)
                add_element(r, n);
            break;
        case ONLY_CHILD_FILTER:
            if(siblings->count == 1)
                add_element(r, n);
            break;
        case NTH_CHILD_FILTER:
            m = (filter_s->value).s->multiplier;
            o = (filter_s->value).s->offset;
            pos = get_element_pos(siblings, n) + 1;

            if(m == 0 && o == 0) break;
            else if (m == 0 && o == pos) add_element(r, n);
            else if (m != 0 && (pos - o) % m == 0 && (pos - o) / m >= 0) add_element(r, n);
            break;

        case NTH_LAST_CHILD_FILTER:
            m = filter_s->value.s->multiplier;
            o = filter_s->value.s->offset;
            pos = siblings->count - get_element_pos(siblings, n);

            if(m == 0 && o == 0) break;
            else if (m == 0 && o == pos) add_element(r, n);
            else if (m != 0 && (pos - o) % m == 0 && (pos - o) / m >= 0) add_element(r, n);
            break;

        case EMPTY_FILTER:
            children = filter_nodes_by_type(get_children(n), ELEMENT);
            if(children == NULL || children->count == 0)
                add_element(r, n);
	    destroy_generic_list(children);
            break;
        case NOT_FILTER:
            if(get_element_pos(not_nodes, n) < 0) add_element(r, n);
            break;
        default:
            log(F, "Only simple pseudo filters are implemented.\n");
        }

	destroy_generic_list(siblings);
	siblings = NULL;
    }


    return r;
}

list* filter_nodes_by_selector(const list* nodes, selector* s, const list* all_nodes){
  list* r = duplicate_generic_list(nodes), *old;
  int i;

  if(s->id != NULL){
    old = r;
    r = filter_nodes_by_name(r, s->id);
    destroy_generic_list(old);
  }

  if(s->attrs != NULL){
    for(i = 0; i < s->attrs->count; i++){
      old = r;
      r = filter_nodes_by_attr(r, get_element_at(s->attrs, i));
      destroy_generic_list(old);
    }
  }

  if(s->filters != NULL){
    for(i = 0; i < s->filters->count; i++){
      old = r;
      r = filter_nodes_by_pseudo_filter(r, get_element_at(s->filters, i), all_nodes);
      destroy_generic_list(old);
    }
  }

  return r;
}

list* run_query(queue* query, const list* all_nodes){
    selector* s;
    list* nodes = duplicate_generic_list(all_nodes), *old;
    list* result = new_generic_list(1);
    tree_root* rbtree = new_simple_rbtree();

    int op, *holder;

    while(query->count > 0){
        switch(peek_queue_type(query)){
        case LXQ_RELATION_TYPE:
	      holder = ((int*)dequeue(query));
	      op = *holder;
	      if(op == ','){
	        //	    result = merge_lists(result, nodes);
	        generic_list_iterator* it = new_generic_list_iterator(nodes);
	        while(generic_list_iterator_has_next(it)){
	          void* aux = generic_list_iterator_next(it);
	          void* old = rb_tree_insert(rbtree, aux);
	          if(!old)
		    add_element(result, aux);
	        }
	        destroy_generic_list_iterator(it);
	        destroy_generic_list(nodes);
	        nodes = duplicate_generic_list(all_nodes);
	      }
	      else{
	        old = nodes;
	        nodes = apply_operator(nodes, op);
	        destroy_generic_list(old);
	      }
	      free(holder);
	      break;
        case LXQ_SELECTOR_TYPE:
	      s =(selector*)dequeue(query);
	      list* old = nodes;
	      nodes = filter_nodes_by_selector(nodes, s, all_nodes);
	      destroy_generic_list(old);
	      destroy_selector(s);
	      break;
        }
    }

    destroy_generic_list(query);
    //    return remove_duplicates(merge_lists(result, nodes));
    generic_list_iterator* it = new_generic_list_iterator(nodes);
    while(generic_list_iterator_has_next(it)){
      void* aux = generic_list_iterator_next(it);
      void* old = rb_tree_insert(rbtree, aux);
      if(!old)
	    add_element(result, aux);
    }
    destroy_generic_list_iterator(it);
    destroy_generic_list(nodes);
    destroy_rbtree(rbtree);
    return result;

}

list* query(char* query_string, dom_node* node){
    list* all_nodes = get_descendants(node);

    if(!all_nodes)
      all_nodes = new_generic_list(1);

    add_element(all_nodes, node);
    queue* query = parse_query(query_string);

    list* r = run_query(query, all_nodes);
    destroy_generic_list(all_nodes);
    return r;
}

