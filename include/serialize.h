#ifndef _LXQ_SERIALIZE_H_
#define _LXQ_SERIALIZE_H_

//TODO: There this cross platform definition must be configured through some sort of './configure' script
#define MONGO_USE_LONG_LONG_INT

//#include "node.h"
#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "bdom.h"

typedef enum{
    XML=0,
    JSON=1,
    YAML=2
} serialization_type;

extern char* node_to_string(dom_node* root, serialization_type t);

extern struct bdom_s* serialize_dom_doc(doc* document);
#endif

