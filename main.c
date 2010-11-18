#include <stdio.h>
#include <string.h>
#include "include/node.h"
#include "include/serialize.h"
#include "include/dom_parser.h"
#include "include/stack.h"
#include "include/query_runner.h"
#include "include/huffman.h"



int main(int argc, char** argv){
  doc* document = parse_dom("test.xml");
  char* cha = node_to_string(document->root, XML);

  //cha = "abcdefg";
  printf("%s", huffman_decode(huffman_encode(cha, strlen(cha))));



  //output_xml(document);
  //printf("%s", cha);

  //free(cha);


  /*if(argc != 2){
    printf("usage: %s \"query\"\n", argv[0]);

      return 0;
  }
  doc* document = parse_dom(argv[1]);
  //bdom* b = serialize_dom_doc(document);

  //  dump(b->bb->buffer, 0, b->bb->size);
  //printf("size %d\n", b->bb->size);

  //destroy_bdom(b);

  list* result = query(argv[2],document->root);

  //printf("List size is %d\n", result->count);
  int i;
  char* cha;

  /*for(i=0; i < result->count; i++){
    dom_node* t = (dom_node*)get_element_at(result, i);
    cha = node_to_string(t, XML);
    printf("%s", cha);
    free(cha);
  }*/
  printf("Results: %d\n", result->count);
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
  return 0;
}

