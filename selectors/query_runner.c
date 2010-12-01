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

#include "../include/query_runner.h"
#include "../include/stack.h"
#include "../include/node.h"
#include "../include/lxq_parser.h"
#include "../include/y.tab.h"
#include "../include/query_parser.h"
#include "../include/eregex.h"
#include "../include/byte_buffer.h"
#include "../include/custom_selectors.h"

list* run_query(queue* query, const list* all_nodes);
int match_str(const char* str, const match_value* op);

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

list* apply_custom_relation_operator(const list* nodes, char* op){
    list* (*operator)(list*) = get_simple_custom_operator_by_name(op);

    if(operator == NULL){
      log(W, "Custom operator %s is not registered", op);
      return NULL;
    }


    list* r = operator((list*) nodes);
    return r;
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
	            if(next_node != NULL){
                    void* old = rb_tree_insert(rbtree, next_node);
                    if(!old)
                        add_element(result, next_node);
	            }
                break;
            }
            default:
                log(W, "Relation operator %d, is not implemented.", op);
        }
    }
    destroy_rbtree(rbtree);
    return result;
}

list* filter_nodes_by_name(const list* nodes, match_value* name){
    int i;
    if(nodes == NULL) return NULL;
    if(nodes->count == 0) return NULL;

    list* r = new_generic_list(nodes->capacity);

    for(i = 0; i < nodes->count; i++){
        if(match_str(((dom_node*)get_element_at(nodes, i))->name, name)){
            add_element(r, get_element_at(nodes, i));
        }
    }

    return r;
}

int match_str(const char* str, const match_value* op){
    if(str == NULL || op == NULL || op->value == NULL){ return 1; }

    switch(op->op){
    case CONTAINS_OP:
    case REGEX_OP:
        return match(str, op->value, 0);
        break;
    case REGEXI_OP:
        return match(str, op->value, 1);
        break;
    case EQUAL_OP:
        return !strcmp(str, op->value);
        break;
    case NOTEQUAL_OP:
        return strcmp(str, op->value);
        break;
    default:
        log(F, "Unknown, (unimplemented?) operator: %d.", op->op);
        exit(1);
    }
}

list* get_matching_attr_by_name(dom_node* node, match_value* name){
    list* r = NULL;
    dom_node* attr;

    if(name->op == EQUAL_OP){
        attr = get_attribute_by_name(node, name->value);
        if(attr == NULL) return NULL;
        else{
            r = new_generic_list(1);
            add_element(r, attr);
        }
    }
    else if(node->attributes != NULL){
        tree_iterator* ti = new_tree_iterator(node->attributes);
        r = new_generic_list(1);
        while(tree_iterator_has_next(ti)){
            dom_node* attr = (dom_node*)tree_iterator_next(ti);
            if(match_str(attr->name, name)){
                add_element(r, attr);
            }
        }
        destroy_iterator(ti);
    }

    return r;
}


list* filter_nodes_by_attr(const list* nodes, attr_selector* attr_s){
    int i;
    if(nodes == NULL) return NULL;
    if(nodes->count == 0) return NULL;

    list* r = new_generic_list(nodes->capacity);

    for(i = 0; i < nodes->count; i++){
        dom_node* n = get_element_at(nodes, i);

        stack* matching_attr = get_matching_attr_by_name(n, attr_s->name);
        if(matching_attr && matching_attr->count > 0){
            if(attr_s->value == NULL){
                add_element(r, n);
            }
            else{
                while(matching_attr->count > 0){
                    dom_node* attr = (dom_node*)pop_stack(matching_attr);

                    if(match_str(attr->value, attr_s->value)){
                        add_element(r, n);
                        break;
                    }
                }
            }
        }
        destroy_generic_list(matching_attr);
    }

    return r;
}

list* filter_nodes_by_pseudo_filter(const list* nodes, filter_selector* filter_s, const list* all_nodes){
    int i, m, o, pos;
    dom_node* n;
    list* siblings = NULL;
    list* children;
    int (*s_filter)(dom_node*);
    int (*filter)(dom_node*, list*);


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
        case SCUSTOM_FILTER:
            s_filter = get_simple_custom_filter_by_name(filter_s->name);
            if(s_filter != NULL){
                if(s_filter(n)) add_element(r, n);
            }
            else{
                log(W, "The filter '%s' is not registered as a simple filter.", filter_s->name);
                i = nodes->count;
            }
            break;
        case CUSTOM_FILTER:
            filter = get_custom_filter_by_name(filter_s->name);
            if(filter != NULL){
                if(filter(n, filter_s->args)) add_element(r, n);
            }
            else{
                log(W, "The filter '%s' is not registered as a full filter.", filter_s->name);
                i = nodes->count;
            }
            break;
        default:
            log(F, "The filter is not implemented.");
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
        int type = peek_queue_type(query);
        switch(type){
        case CUSTOM_RELATION_OPERATOR:
        case LXQ_RELATION_TYPE:
	      holder = ((int*)dequeue(query));
	      op = *holder;
	      if(op == ','){
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
	      else if(type == CUSTOM_RELATION_OPERATOR){
            old = nodes;
            nodes = apply_custom_relation_operator(nodes, (char*)holder);
            destroy_generic_list(old);
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

list* query(const char* query_string, dom_node* node){
    char* new_query;

    if(!match(query_string, "^@", 0)){
        byte_buffer* bb = new_byte_buffer(strlen(query_string) + 1);
        append_string_to_buffer("@", bb);
        append_string_to_buffer(query_string, bb);
        append_bytes_to_buffer("\0", bb, 1);
        new_query = bb->buffer;
        free(bb);
    }
    else{
        new_query = strdup(query_string);
    }

    queue* query = parse_query(new_query);

    if(!query){
        log(W, "Query '%s' is not a valid query.", query_string);
        free(new_query);
        return NULL;
    }

    list* all_nodes = get_descendants(node);
    if(!all_nodes)
      all_nodes = new_generic_list(1);

    add_element(all_nodes, node);

    list* r = run_query(query, all_nodes);
    destroy_generic_list(all_nodes);
    free(new_query);
    return r;
}

