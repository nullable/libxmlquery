#ifndef __BYTE_BUFFER_H__
#define __BYTE_BUFFER_H__

#include <stdint.h>

typedef struct bb_s{
    char* buffer;
    int32_t capacity;
    int32_t size;
} byte_buffer;

extern void destroy_byte_buffer(byte_buffer* bb);
extern byte_buffer* new_byte_buffer(int32_t initial);
extern void append_string_to_buffer(const char* s, byte_buffer* b);
extern void append_bytes_to_buffer(const char* bytes, byte_buffer* b, int32_t size);
extern void append_buffer_to_buffer(byte_buffer* b2, byte_buffer* b);

#endif

