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

