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
#include "include/node.h"
#include "include/serialize.h"
#include "include/lxq_parser.h"
#include "include/stack.h"
#include "include/query_runner.h"
#include "include/node.h"

void dump(char* array, int offset, int size){
  for(; offset < size; offset++)
    printf("%c", array[offset]);
  putchar('\n');
}

int alberto(dom_node* alberto, list* l){
    return 1;
}

int main(int argc, char** argv){

  //  char* cha = node_to_string(document->root, JSON);

  //output_xml(document);
  //printf("%s", cha);

  //free(cha);

  if(argc != 3){
    printf("usage: %s \"xml_file\" \"query\"\n", argv[0]);
      return 0;
  }
  register_custom_filter("alberto", &alberto);
  doc* document = parse_xml(argv[1]);

  //printf("%s", node_to_string(document->root, XML));

  //bdom* b = serialize_dom_doc(document);

  //  dump(b->bb->buffer, 0, b->bb->size);
  //printf("size %d\n", b->bb->size);

  //destroy_bdom(b);

  list* result = query(argv[2],document->root);

  //printf("List size is %d\n", result->count);
  int i;
  char* cha;

  for(i=0; i < result->count; i++){
    dom_node* t = (dom_node*)get_element_at(result, i);
    cha = node_to_string(t, XML);
    printf("%s", cha);
    free(cha);
  }
  //  printf("Results: %d\n", result->count);
  if(result)
    destroy_generic_list(result);
  /*
  printf("\n\n\n==========================================================\n\n\n");
  cha = node_to_string(document->root, JSON);
  printf("%s", cha);

  free(cha);*/
  if(document != NULL)
    destroy_dom_tree(document);

  /*destroy_generic_list(result);
  list* cenas = parse_query("@E");
  int i = 0;
  for(; i < cenas->count; i++){
    free(get_element_at(cenas, i));
  }
  destroy_generic_list(cenas);*/
  /*  printf("\n\n\nAGORA DE UMA STRING\n");
  yy_scan_string("<this is=\"a test\">texto</this>");
  yyparse();

  printf("==========================================================\n");
  output_xml(document);  */

  /*  int a, b, c, d, e;

  stack* s = new_stack(2);

  printf("pushing %p\n", &a);
  push_stack(s, &a);
  printf("pushing %p\n", &b);
  push_stack(s, &b);
  printf("pushing %p\n", &c);
  push_stack(s, &c);
  printf("pushing %p\n", &d);
  push_stack(s, &d);
  printf("pushing %p\n", &e);
  push_stack(s, &e);

  printf("\n\n==poping==\n\n");

  printf("poping %p\n", pop_stack(s));
  printf("poping %p\n", pop_stack(s));
  printf("poping %p\n", pop_stack(s));
  printf("poping %p\n", pop_stack(s));
  printf("poping %p\n", pop_stack(s));

  destroy_stack(s);*/
  destroy_dictionary();
  destroy_custom_filters();
  return 0;
}

