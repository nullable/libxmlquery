#include "dom_parser.h"

extern int yyparse();

void parse_dom(){
  yyparse();
}
