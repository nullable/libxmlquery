#ifndef _LXQ_SERIALIZE_H_
#define _LXQ_SERIALIZE_H_

//TODO: There this cross platform definition must be configured through some sort of './configure' script
#define MONGO_USE_LONG_LONG_INT

//#include "node.h"
#include <stdlib.h>
#include <stdio.h>
#include "bson.h"
#include "node.h"


typedef enum{
    XML=0,
    JSON=1,
    YAML=2
} serialization_type;

typedef struct bb_s{
    char* buffer;
    int capacity;
    int size;
} byte_buffer;

extern char* node_to_string(dom_node* root, serialization_type t);
extern void append_bytes_to_buffer(const char* bytes, byte_buffer* b, size_t size);
extern void destroy_byte_buffer(byte_buffer* bb);

#endif

