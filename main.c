#include <stdio.h>
#include "dom/node.h"
#include "dom/serialize.h"
#include "parser/dom_parser.h"
#include "data_structures/stack.h"
#include "selectors/query_runner.h"

int main(int argc, char** argv){
  doc* document = parse_dom("test.xml");
  //char* cha = document_to_string(document, JSON);
  
  //output_xml(document);
  //printf("%s", cha);

  //free(cha);

  if(argc != 2){
    printf("usage: %s \"query\"\n", argv[0]);
      return 0;
  }

  list* result = query(argv[1],document->root);

  //printf("List size is %d\n", result->count);
  int i;
  for(i=0; i < result->count; i++){
    dom_node* t = (dom_node*)get_element_at(result, i);
    set_name(t, "banana");
    printf("Result node:\n");
    printf("%s", node_to_string(t, XML));
  }

  printf("\n\n\n==========================================================\n\n\n");
  char *cha = node_to_string(document->root, JSON);
  printf("%s", cha);

  free(cha);
  if(document != NULL)
    destroy_dom_tree(document);

    /*  list* cenas = parse_query("@E");
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

