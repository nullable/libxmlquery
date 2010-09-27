#include "parser.h"

extern int yyparse();

void parse_dom(){
  yyparse();
}
