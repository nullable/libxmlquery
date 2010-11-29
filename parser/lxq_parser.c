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
 
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../include/y.tab.h"
#include "../include/stack.h"
#include "../include/macros.h"
#include "../include/lxq_parser.h"

extern doc* lxq_document;
extern list* lxq_selected_elements;
extern stack* trash_bin;
extern void parse_file(char*);
extern void parse_string(const char*);

doc* parse_xml(char* filename){
  doc* document;

  trash_bin = new_stack(16);

  parse_file(filename);

  while(trash_bin->count > 0){
    free(pop_stack(trash_bin));
  }
  destroy_generic_list(trash_bin);

  if(lxq_document == NULL)
    return NULL;

  document = alloc(doc, 1);
  memcpy(document, lxq_document, sizeof(doc));
  free(lxq_document);
  lxq_document = NULL;
  return document;
}

list* parse_query(const char* query){
  list* sles;

  trash_bin = new_stack(16);

  parse_string(query);

  while(trash_bin->count > 0){
    free(pop_stack(trash_bin));
  }
  destroy_generic_list(trash_bin);

  if(lxq_selected_elements == NULL)
    return NULL;

  sles = alloc(list, 1);
  memcpy(sles, lxq_selected_elements, sizeof(list));
  free(lxq_selected_elements);
  lxq_selected_elements = NULL;
  return sles;
}

doc* parse_xml_from_string(const char* xmlstring){
  doc* document;

  trash_bin = new_stack(16);

  parse_string(xmlstring);

  while(trash_bin->count > 0){
    free(pop_stack(trash_bin));
  }
  destroy_generic_list(trash_bin);

  if(lxq_document == NULL)
    return NULL;

  document = alloc(doc, 1);
  memcpy(document, lxq_document, sizeof(doc));
  free(lxq_document);
  lxq_document = NULL;
  return document;
}

