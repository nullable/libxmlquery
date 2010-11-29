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

