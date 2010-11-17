#include "../include/btree.h"

btree* new_btree(){
    btree* r = alloc(btree, 1);
    r->value = NULL;
    r->left = NULL;
    r->right = NULL;

    return r;
}

