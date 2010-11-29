#include <stdio.h>
#include <string.h>
#include "node.h"
#include "serialize.h"
#include "lxq_parser.h"
#include "stack.h"
#include "query_runner.h"
#include "node.h"

void dump(char* array, int offset, int size){
  for(; offset < size; offset++)
    printf("%c", array[offset]);
  putchar('\n');
}

int filter(dom_node* filter, list* l){
    return 1;
}

int main(int argc, char** argv){

  if(argc != 3){
    printf("usage: %s \"xml_file\" \"query\"\n", argv[0]);
      return 0;
  }
  register_custom_filter("filter", &filter);
  doc* document = parse_xml(argv[1]);


  list* result = query(argv[2],document->root);


  int i;
  char* cha;

  for(i=0; i < result->count; i++){
    dom_node* t = (dom_node*)get_element_at(result, i);
    cha = node_to_string(t, XML);
    printf("%s", cha);
    free(cha);
  }

  if(result)
    destroy_generic_list(result);
  if(document != NULL)
    destroy_dom_tree(document);

  destroy_dictionary();
  destroy_custom_filters();
  return 0;
}

