#ifndef __BITBUFFER_H__
#define __BITBUFFER_H__


typedef struct bitbuffer_s{
    unsigned char* buffer;
    //in bytes
    unsigned int capacity;

    //in bits
    unsigned int size;
} bitbuffer;

extern bitbuffer* new_bitbuffer(unsigned int byte_size);
extern bitbuffer* duplicate_bitbuffer(bitbuffer* bb);
extern void append_bit_to_buffer(unsigned char c, bitbuffer* bb);
extern void append_bits_to_buffer(unsigned int c, unsigned int bit_count, bitbuffer* bb);
extern void append_bitbuffer_to_bitbuffer(const bitbuffer* from, bitbuffer* to);
extern char get_bit_from_buffer(const bitbuffer* bb, unsigned int offset);
extern char get_byte_from_buffer(const bitbuffer* bb, unsigned int offset);
int get_bits_from_buffer(const bitbuffer* bb, unsigned int offset, unsigned int bit_count);


extern void dump_bit_buffer(bitbuffer* b);
#endif

