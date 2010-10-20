#include <stdio.h>
#include "dom/node.h"
#include "dom/serialize.h"
#include "parser/dom_parser.h"
#include "data_structures/stack.h"

int main(){
  doc* document = parse_dom("test.xml");

  //printf("\n\n\n==========================================================\n\n\n");
  //output_xml(document);
  printf("%s", document_to_string(document, JSON));

  if(document != NULL)
    destroy_dom_tree(document);

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

