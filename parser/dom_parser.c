#include <stdlib.h>
#include "../data_structures/stack.h"
#include "dom_parser.h"

extern int yyparse();
extern void yylex_destroy();
extern stack* trash_bin;

void parse_dom(){

  trash_bin = new_stack(16);
  yyparse();
  yylex_destroy();

  while(trash_bin->count > 0){
    free(pop_stack(trash_bin));
  }
  destroy_stack(trash_bin);
}
