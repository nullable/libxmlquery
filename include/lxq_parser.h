#ifndef __DOM_PARSER_H__
#define __DOM_PARSER_H__

#include "node.h"

extern doc* parse_xml(char* filename);
extern list* parse_query(const char* query);
extern doc* parse_xml_from_string(const char* xmlstring);
#endif
