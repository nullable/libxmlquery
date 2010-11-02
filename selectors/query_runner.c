#include <string.h>

#include "query_runner.h"
#include "../data_structures/stack.h"
#include "../dom/node.h"
#include "../parser/dom_parser.h"
#include "../parser/y.tab.h"
#include "query_parser.h"

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
    list* nodes = get_descendants(n);
    return filter_nodes_by_type(nodes, ELEMENT);
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


list* apply_operator(list* nodes, int op){
    int i;
    dom_node* next_node;
    list* result = new_generic_list(1);
    for(i = 0; i < nodes->count; i++){
        dom_node* node = get_element_at(nodes, i);
        switch(op){
        case SPACE:
            result = merge_lists(result, get_xml_descendants(node));
            break;
        case '>':
            result = merge_lists(result, get_xml_children(node));
            break;
        case '~':
            result = merge_lists(result, get_xml_nodes_after(node));
            break;
        case '+':
            next_node = get_xml_node_after(node);
            if(next_node != NULL) add_element(result, next_node);
            break;
        }
    }

    return remove_duplicates(result);
}

list* filter_nodes_by_name(list* nodes, char* name){
    int i;
    if(nodes == NULL) return NULL;
    if(nodes->count == 0) return nodes;

    list* r = new_generic_list(nodes->capacity);

    for(i = 0; i < nodes->count; i++){
        if(!strcmp(name, ((dom_node*)get_element_at(nodes, i))->name)){
            add_element(r, get_element_at(nodes, i));
        }
    }

    return r;
}

list* filter_nodes_by_attr(list* nodes, attr_selector* attr_s){
    int i;

    if(nodes == NULL) return NULL;
    if(nodes->count == 0) return nodes;

    list* r = new_generic_list(nodes->capacity);

    for(i = 0; i < nodes->count; i++){
        dom_node* n = get_element_at(nodes, i);
        dom_node* attr = get_attribute_by_name(n, attr_s->name);
        if(attr == NULL || strcmp(attr->name, attr_s->name)) continue;
        switch(attr_s->op){
        case EQUAL_OP:
            printf("%s == %s\n", attr->value, attr_s->value);
            if(!strcmp(attr->value, attr_s->value)){
                add_element(r, n);
            }
            break;
        case NO_OP:
                add_element(r, n);
            break;
        default:
            log(F, "Regex operators not implemented.\n");
        }
    }

    return r;
}

list* filter_nodes_by_pseudo_filter(list* nodes, filter_selector* filter_s){
    int i;
    dom_node* n;
    list* siblings;
    list* children;

    if(nodes == NULL) return NULL;
    if(nodes->count == 0) return nodes;

    list* r = new_generic_list(nodes->capacity);

    for(i = 0; i < nodes->count; i++){
        switch(filter_s->op){
        case FIRST_CHILD_FILTER:
        case LAST_CHILD_FILTER:
        case ONLY_CHILD_FILTER:
            n = get_element_at(nodes, i);
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
        case EMPTY_FILTER:
            children = filter_nodes_by_type(get_children(n), ELEMENT);
            if(children == NULL || children->count == 0)
                add_element(r, n);
            break;
        default:
            log(F, "Only simple pseudo filters are implemented.\n");
        }
    }

    return r;
}

list* filter_nodes_by_selector(list* nodes, selector* s){
    list* r = nodes;
    int i;

    if(s->id != NULL){
        r = filter_nodes_by_name(nodes, s->id);
    }

    if(s->attrs != NULL){
        for(i = 0; i < s->attrs->count; i++){
            printf("%s\n", ((attr_selector*)get_element_at(s->attrs, i))->name);
            r = filter_nodes_by_attr(r, get_element_at(s->attrs, i));
        }
    }

    if(s->filters != NULL){
        for(i = 0; i < s->filters->count; i++){
            r = filter_nodes_by_pseudo_filter(r, get_element_at(s->filters, i));
        }
    }

    return r;
}

list* query(char* query_string, dom_node* node){
    list* all_nodes = get_descendants(node);
    int op;
    add_element(all_nodes, node);


    selector* s;
    list* nodes = all_nodes;
    list* result = new_generic_list(1);

    queue* query = parse_query(query_string);

    while(query->count > 0){
        switch(peek_queue_type(query)){
        case LXQ_RELATION_TYPE:
	        op = *((int*)dequeue(query));
            if(op == ','){
                result = merge_lists(result, nodes);
                nodes = duplicate_generic_list(all_nodes);
            }
            else{
                list* old_nodes = nodes;
                nodes = apply_operator(nodes, op);
                destroy_generic_list(old_nodes);
            }
            break;
        case LXQ_SELECTOR_TYPE:
            s =(selector*)dequeue(query);
            nodes = filter_nodes_by_selector(nodes, s);
            //TODO: memory leak
            break;
        }
    }
    print_node_list(nodes);
    return remove_duplicates(merge_lists(result, nodes));
}

