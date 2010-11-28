#ifndef _LXQ_SERIALIZE_H_
#define _LXQ_SERIALIZE_H_

#include <stdlib.h>
#include <stdio.h>
#include "node.h"

typedef enum{
    XML=0,
    JSON=1,
    YAML=2
} serialization_type;

extern char* node_to_string(dom_node* root, serialization_type t);
#endif

