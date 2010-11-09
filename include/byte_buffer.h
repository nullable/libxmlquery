#ifndef __BYTE_BUFFER_H__
#define __BYTE_BUFFER_H__
typedef struct bb_s{
    char* buffer;
    int capacity;
    int size;
} byte_buffer;

extern void destroy_byte_buffer(byte_buffer* bb);
extern byte_buffer* new_byte_buffer(int initial);
extern void append_string_to_buffer(const char* s, byte_buffer* b);
extern void append_bytes_to_buffer(const char* bytes, byte_buffer* b, size_t size);
extern void append_buffer_to_buffer(byte_buffer* b2, byte_buffer* b);

#endif
