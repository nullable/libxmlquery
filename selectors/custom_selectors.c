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
#include "../include/macros.h"
#include "../include/rbtree.h"
#include "../include/custom_selectors.h"

/* rbtree functions */

const void* get_custom_filter_key(const struct stree_node* node){
    return ((custom_filter*)node->node)->name;
}

const void* get_custom_operator_key(const struct stree_node* node){
    return ((custom_operator*)node->node)->name;
}

/* static holders */

tree_root* get_custom_filters(){
    static tree_root* custom_filters = NULL;

    if(custom_filters == NULL)
        custom_filters = new_rbtree(&get_custom_filter_key, &compare_string);

    return custom_filters;
}

tree_root* get_custom_operators(){
    static tree_root* custom_operators = NULL;

    if(custom_operators == NULL)
        custom_operators = new_rbtree(&get_custom_operator_key, &compare_string);

    return custom_operators;
}

/* initializers */

custom_filter* new_custom_filter(const char* name){
    custom_filter* r = alloc(custom_filter, 1);
    r->name = strdup(name);
    r->function.simple = NULL;
    r->function.complex = NULL;
    return r;
}

custom_operator* new_custom_operator(const char* name){
    custom_operator* r = alloc(custom_operator, 1);
    r->name = strdup(name);
    r->function.simple = NULL;
    r->function.complex = NULL;
    return r;
}

/* registers */

void register_simple_custom_filter(const char* name, int (*filter)(dom_node* node)) {
    tree_root* lxq_custom_filters = get_custom_filters();

    custom_filter* r = new_custom_filter(name);
    r->function.simple = filter;
    rb_tree_insert(lxq_custom_filters, r);
}

void register_custom_filter(const char* name, int (*filter)(dom_node* node, list* args)){
    tree_root* lxq_custom_filters = get_custom_filters();

    custom_filter* r = new_custom_filter(name);
    r->function.complex = filter;
    rb_tree_insert(lxq_custom_filters, r);
}

void register_simple_custom_operator(const char* name, list* (*operator)(list* nodes)) {
    tree_root* lxq_custom_operators = get_custom_operators();

    custom_operator* r = new_custom_operator(name);
    r->function.simple = operator;
    rb_tree_insert(lxq_custom_operators, r);
}

void register_custom_operator(const char* name, list* (*operator)(list* nodes, list* args)){
    tree_root* lxq_custom_operators = get_custom_operators();

    custom_operator* r = new_custom_operator(name);
    r->function.complex = operator;
    rb_tree_insert(lxq_custom_operators, r);
}

/* getters */

int (*get_simple_custom_filter_by_name(const char* name))(dom_node*){
    custom_filter* cf = (custom_filter*)search_rbtree(get_custom_filters(), name);
    return (cf != NULL)? cf->function.simple : NULL;
}

int (*get_custom_filter_by_name(const char* name))(dom_node*, list* args){
    custom_filter* cf = (custom_filter*)search_rbtree(get_custom_filters(), name);
    return (cf != NULL)? cf->function.complex : NULL;
}

list* (*get_simple_custom_operator_by_name(const char* name))(list* nodes){
    custom_operator* cf = (custom_operator*)search_rbtree(get_custom_operators(), name);
    return (cf != NULL)? cf->function.simple : NULL;
}

list* (*get_custom_operator_by_name(const char* name))(list* nodes, list* args){
    custom_operator* cf = (custom_operator*)search_rbtree(get_custom_operators(), name);
    return (cf != NULL)? cf->function.complex : NULL;
}

/* destroys*/

void destroy_custom_filters(){
    tree_root* lxq_custom_filters = get_custom_filters();

    tree_iterator* ti = new_tree_iterator(lxq_custom_filters);

    while(tree_iterator_has_next(ti)){
        custom_filter* cf = (custom_filter*)tree_iterator_next(ti);
        free(cf->name);
        free(cf);
    }
    destroy_iterator(ti);
    destroy_rbtree(lxq_custom_filters);
}

void destroy_custom_operators(){
    tree_root* lxq_custom_operators = get_custom_operators();

    tree_iterator* ti = new_tree_iterator(lxq_custom_operators);

    while(tree_iterator_has_next(ti)){
        custom_operator* cf = (custom_operator*)tree_iterator_next(ti);
        free(cf->name);
        free(cf);
    }
    destroy_iterator(ti);
    destroy_rbtree(lxq_custom_operators);
}

/* custom operators */

list* eo_first(list* nodes){
    if(nodes == NULL || nodes->count < 1) return NULL;

    list* r = new_generic_list(1);
    add_element(r, get_element_at(nodes, 0));
    return r;
}

list* eo_text_children(list* nodes){
    if(nodes == NULL || nodes->count < 1) return NULL;

    list* r = new_generic_list(1);
    int i, j;
    for(i = 0; i < nodes->count; i++){
        dom_node* node = (dom_node*)get_element_at(nodes, i);
        if(node->type != ELEMENT || node->children == NULL) continue;

        for(j = 0; j < node->children->count; j++){
            dom_node* child = (dom_node*)get_element_at(node->children, j);
            if(child->type == TEXT_NODE)
                add_element(r, child);
        }
    }

    return r;
}

void register_extended_operators(){
    register_simple_custom_operator("first", &eo_first);
    register_simple_custom_operator("text-children", &eo_text_children);
}

