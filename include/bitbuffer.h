#ifndef __BITBUFFER_H__
#define __BITBUFFER_H__


typedef struct bitbuffer_s{
    char* buffer;
    //in bytes
    int capacity;

    //in bits
    int size;
} bitbuffer;

extern bitbuffer* new_bitbuffer(int byte_size);
extern bitbuffer* duplicate_bitbuffer(bitbuffer* bb);
extern void append_bit_to_buffer(char c, bitbuffer* bb);
extern void append_bits_to_buffer(char c, int bit_count, bitbuffer* bb);
extern void append_bitbuffer_to_bitbuffer(const bitbuffer* from, bitbuffer* to);
extern char get_bit_from_buffer(const bitbuffer* bb, int offset);
extern char get_byte_from_buffer(const bitbuffer* bb, int offset);

#endif

