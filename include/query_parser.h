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
 
#ifndef __QUERY_PARSER_H__
#define __QUERY_PARSER_H__

#define LXQ_RELATION_TYPE 0
#define LXQ_SELECTOR_TYPE 1

#include "stack.h"

struct selector_s;

typedef struct match_value_s{
    char* value;
    int op;
} match_value;

typedef struct attr_selector_s{
  match_value* name;
  match_value* value;
} attr_selector;

typedef struct step_s{ int multiplier; int offset; } step;

typedef struct filter_selector_s{
  int op;
  char* name;
  list* args;
  struct {
    step* s;
    struct generic_list_s* selector;
  } value;
} filter_selector;

typedef struct selector_s{
  match_value* id;
  stack* attrs;
  stack* filters;
} selector;

extern selector* new_selector(match_value* id);
extern attr_selector* new_attr_value_selector(match_value* name, match_value* value);
extern filter_selector* new_filter(int filter);
extern match_value* new_match_value(const char* value, int op);
extern match_value* new_match_value_no_strdup(char* value, int op);

extern void destroy_selector(selector* s);
extern void destroy_filter_selector(filter_selector* fs);
extern void destroy_attr_selector(attr_selector* as);

extern match_value* make_operators(char* str, int op);
#endif

