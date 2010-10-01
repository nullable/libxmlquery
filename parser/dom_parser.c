#include "dom_parser.h"

extern int yyparse();
extern void yylex_destroy();

void parse_dom(){
  yyparse();
  yylex_destroy();
}
