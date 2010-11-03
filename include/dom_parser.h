#ifndef __DOM_PARSER_H__
#define __DOM_PARSER_H__

#include "node.h"

extern doc* parse_dom(char* filename);
extern list* parse_query(const char* query);
#endif
