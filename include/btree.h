#ifndef __BTREE_H__
#define __BTREE_H__

#include "macros.h"
#include "stdlib.h"

typedef struct btree_s{
    void* value;
    struct btree_s* left;
    struct btree_s* right;
} btree;

//typedef struct btree_iterator_s{
//    btree* cur;
//    int pos;
//} btree_iterator;

btree* new_btree();

#endif

