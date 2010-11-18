#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/macros.h"
#include "../include/bitbuffer.h"


bitbuffer* new_bitbuffer(unsigned int byte_size){
    bitbuffer* r = alloc(bitbuffer, 1);
    r->buffer = alloc(unsigned char, byte_size);
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
    unsigned int i;
    for(i = 0; i < from->size; i++){
        append_bit_to_buffer(get_bit_from_buffer(from, i), to);
    }
}

void print_bitbuffer(bitbuffer* bb){
    unsigned int j;

    for(j = 0; j < bb->size; j++){
        printf("%d", get_bit_from_buffer(bb, j));
    }
    printf("\n");
}

void append_bit_to_buffer(unsigned char c, bitbuffer* bb){
    unsigned char v = c & (unsigned char) 0x01;
    int bit_offset = bb->size % 8;
    unsigned char* cur = bb->buffer + (bb->size / 8);
    //char* cur = bb->buffer + (bb->size / 8);

    //for(i = 0; i < bit_offset; i++){ v <<= 1; }

    *cur |= (v << bit_offset);

    bb->size += 1;
    if(bit_offset == 7){
        if(cur - bb->buffer >= bb->capacity){
            bb->buffer = (unsigned char*)realloc(bb->buffer, bb->capacity*2);
            memset(bb->buffer + (bb->size/8), 0, bb->capacity*2 - (bb->size/8));
            bb->capacity *= 2;
        }
    }
}

void dump_bit_buffer(bitbuffer* b){
  int how_many_bytes = b->size / 8 + 1, i;
  printf("Size %d\n", b->size);
  for(i = 0; i < how_many_bytes; i++){
    int offset = 7;
    for(; offset >= 0; offset--)
      if(b->buffer[i] & (((uint8_t) 0x01) << offset))
	printf("1");
      else
	printf("0");
    putchar(' ');
  }
  putchar('\n');
}

void append_bits_to_buffer(unsigned int c, unsigned int bit_count, bitbuffer* bb){
  /*    int i;
    for(i = bit_count; i > 0; i--){
        char v = (char) c >> (i-1);
        append_bit_to_buffer(v, bb);
	}*/

  while(bit_count > 0){
    uint8_t bit_offset = bb->size % 8;
    //how many bits can we eat from c?
    uint8_t mask_offset = (bit_count > 8)? 8 - bit_offset: bit_count;
    unsigned char* cur = bb->buffer + (bb->size / 8);

    //2^bit_offset - 1 = mascara para fazer or
    uint8_t mask = (1 << mask_offset) - 1;
    uint8_t val = c & mask;

    //forward bits (a.k.a eat bits from c)
    c >>= mask_offset;
    
    //Align values of val with the right offset and store those bits.
    *cur |= (val << bit_offset);

    bit_count -= mask_offset;

    bb->size += mask_offset;
    if(bit_offset == 7){
        if(cur - bb->buffer >= bb->capacity){
            bb->buffer = (unsigned char*)realloc(bb->buffer, bb->capacity << 1);
            memset(bb->buffer + (bb->size >> 3), 0, (bb->capacity << 1) - (bb->size >> 3));
            bb->capacity <<= 1;
        }
    }
  }
}


char get_bit_from_buffer(const bitbuffer* bb, unsigned int offset){
    if(offset < 0 || offset > bb->size){
        exit(-1);
    }
    unsigned char* cur = bb->buffer + (offset / 8);
    unsigned int bit_offset = offset % 8;

    return (*cur >> bit_offset) & 1;
}

char get_byte_from_buffer(const bitbuffer* bb, unsigned int offset){
    char r = 0;
    int i;

    for(i = 0; i < 7; i++){
        r |= get_bit_from_buffer(bb, offset+i);
        r <<= 1;
    }
    r |= get_bit_from_buffer(bb, offset+i);
    return r;
}

int get_bits_from_buffer(const bitbuffer* bb, unsigned int offset, unsigned int bit_count){
    int r = 0;
    int i;

    for(i = 0; i < bit_count-1; i++){
        r|= get_bit_from_buffer(bb, offset+i);
        r <<= 1;
    }
    r|= get_bit_from_buffer(bb, offset+i);

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

