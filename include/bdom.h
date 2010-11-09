#ifndef __BDOM_H__
#define __BDOM_H__

#include <stdint.h>
#include "node.h"
#include "byte_buffer.h"

typedef struct bdom_s{
  byte_buffer* bb;
}bdom;

enum bdom_type { BDOM_NAMESPACE,
		 BDOM_NAME,
		 BDOM_NODE,
		 BDOM_ATTR,
		 BDOM_TEXT,
		 BDOM_CDATA,
		 BDOM_VAL,
		 BDOM_DECLARATION,
		 BDOM_DOC		 
};

extern int* get_size_pointer(bdom* bd);
extern uint8_t* get_type_pointer(bdom* bd);
extern bdom* init_bdom(enum bdom_type t);
extern void destroy_bdom(bdom* b);
extern bdom* bdom_from_dom_node(dom_node* d);
extern bdom* bdom_from_string(const char* str, enum bdom_type t);
extern void append_bdom_to_bdom(bdom* from, bdom* to);
#endif
