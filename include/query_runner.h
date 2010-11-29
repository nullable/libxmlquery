#ifndef __QUERY_RUNNER_H__
#define __QUERY_RUNNER_H__

#include "stack.h"
#include "node.h"

typedef struct custom_filter_s{
    char* name;
    struct{
        int (*simple)(dom_node* node);
        int (*args)(dom_node* node, list* args);
    } function;
} custom_filter;

extern list* query(const char*, dom_node*);
extern void register_simple_custom_filter(const char* name, int (*filter)(dom_node* node));
extern void register_custom_filter(const char* name, int (*filter)(dom_node* node, list* args));
extern void destroy_custom_filters();
#endif

