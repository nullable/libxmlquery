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

#ifndef __CUSTOM_SELECTORS_H__
#define __CUSTOM_SELECTORS_H__

#include "stack.h"
#include "node.h"


typedef struct custom_filter_s{
    char* name;
    struct{
        int (*simple)(dom_node* node);
        int (*complex)(dom_node* node, list* args);
    } function;
} custom_filter;

typedef struct custom_operator_s{
    char* name;
    struct{
        list* (*simple)(list* nodes);
        list* (*complex)(list* nodes, list* args);
    } function;
} custom_operator;

/* registers */

extern void register_simple_custom_filter(const char* name, int (*filter)(dom_node* node));
extern void register_custom_filter(const char* name, int (*filter)(dom_node* node, list* args));
extern void register_simple_custom_operator(const char* name, list* (*operator)(list* nodes));

/* getters */

extern int (*get_simple_custom_filter_by_name(const char* name))(dom_node*);
extern int (*get_custom_filter_by_name(const char* name))(dom_node*, list* args);
extern list* (*get_simple_custom_operator_by_name(const char* name))(list*);

/* destroys*/

extern void destroy_custom_filters();
extern void destroy_custom_operators();

/* extend operators */

extern void register_extended_operators();

#endif

