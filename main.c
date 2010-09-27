#include <stdio.h>
#include "dom/node.h"
#include "parser/dom_parser.h"

extern doc* document;

int main(){
  parse_dom();

  printf("==========================================================\n");
  output_xml(document);
  return 0;
}
