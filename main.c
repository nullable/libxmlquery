#include <stdio.h>
#include "dom/node.h"
#include "dom/serialize.h"
#include "parser/dom_parser.h"
#include "data_structures/stack.h"
#include "selectors/query_runner.h"

int main(){
  doc* document = parse_dom("test.xml");
  char* cha = document_to_string(document, JSON);
  //printf("\n\n\n==========================================================\n\n\n");
  //output_xml(document);
  printf("%s", cha);

  free(cha);

  //get adeus
  dom_node* aux = get_element_at(get_children(get_element_at(get_children(document->root), 0)), 0);

  printf("%s\n", aux->name);

  dom_node* attr = delete_attribute(aux, "foo");
  destroy_dom_node(attr);

  /*cha = document_to_string(document, JSON);
  printf("%s", cha);

  free(cha);*/

  list* result = query("@hi",document->root);

  printf("List size is %d\n", result->count);
  int i;
  for(i=0; i < result->count; i++){
    dom_node* t = (dom_node*)get_element_at(result, i);
    printf("%s\n", get_name(t));
  }


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

