/*
Copyright (c) 2010 Frederico Gonçalves, Vasco Fernandes

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
 
#include <stdlib.h>
#include <string.h>
#include "../include/macros.h"
#include "../include/bitbuffer.h"


bitbuffer* new_bitbuffer(int byte_size){
    bitbuffer* r = alloc(bitbuffer, 1);
    r->buffer = alloc(char, byte_size);
    memset(r->buffer, 0, byte_size);
    r->capacity = byte_size;
    r->size = 0;
    return r;
}

bitbuffer* duplicate_bitbuffer(bitbuffer* bb){
    bitbuffer* r = new_bitbuffer(bb->capacity);
    r->size = bb->size;

    //memcpy(r->buffer, bb->buffer, (r->size / 8)+1);
    memcpy(r->buffer, bb->buffer, r->capacity);
    return r;
}

void append_bitbuffer_to_bitbuffer(const bitbuffer* from, bitbuffer* to){
    int i;
    for(i = 0; i < from->size; i++){
        append_bit_to_buffer(get_bit_from_buffer(from, i), to);
    }
}

void print_bitbuffer(bitbuffer* bb){
    int j;

    for(j = 0; j < bb->size; j++){
        printf("%d", get_bit_from_buffer(bb, j));
    }
    printf("\n");
}

void append_bit_to_buffer(char c, bitbuffer* bb){
    char v = c & (unsigned char) 0x01;
    int bit_offset = bb->size % 8;
    char* cur = bb->buffer + (bb->size / 8);
    //char* cur = bb->buffer + (bb->size / 8);

    //for(i = 0; i < bit_offset; i++){ v <<= 1; }

    *cur |= (v << bit_offset);

    bb->size += 1;
    if(bit_offset == 7){
        if(cur - bb->buffer >= bb->capacity){
            bb->buffer = (char*)realloc(bb->buffer, bb->capacity*2);
            memset(bb->buffer + (bb->size/8), 0, bb->capacity*2 - (bb->size/8));
            bb->capacity *= 2;
        }
    }


}

void append_bits_to_buffer(char c, int bit_count, bitbuffer* bb){
    int i;
    for(i = bit_count; i > 0; i--){
        char v = c >> (i-1);
        append_bit_to_buffer(v, bb);
    }

}

char get_bit_from_buffer(const bitbuffer* bb, int offset){
    if(offset < 0 || offset > bb->size){
        exit(-1);
    }
    char* cur = bb->buffer + (offset / 8);
    int bit_offset = offset % 8;

    return (*cur >> bit_offset) & 1;
}

char get_byte_from_buffer(const bitbuffer* bb, int offset){
    char r = 0;
    int i;

    for(i = 0; i < 7; i++){
        r |= get_bit_from_buffer(bb, offset+i);
        r <<= 1;
    }
    r |= get_bit_from_buffer(bb, offset+i);
    return r;
}


/*int main(){
    bitbuffer* bb = new_bitbuffer(1);
    int i, seed = 12244543;
    char c;
    for(i = 0; i < 25; i++){
        c = (char) ((seed >> i) & 1);
        append_bit_to_buffer(c, bb);
        printf("%d", c);
    }
    printf("\n");

    for(i = 0; i < bb->size; i++){
        printf("%d", get_bit_from_buffer(bb, i));
    }
    printf("\n");

    return 0;
}*/

