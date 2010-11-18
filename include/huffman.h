#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include "btree.h"
#include "bitbuffer.h"

typedef struct byte_count{
    int count;
    char byte;
    btree* bt;
    bitbuffer* bb;
} bc;

extern bitbuffer* huffman_encode(char* byte_string, int size);
extern char* huffman_decode(bitbuffer* bit_string);

#endif

