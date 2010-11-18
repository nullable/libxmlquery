#include <string.h>
#include <stdlib.h>
#include "../include/byte_buffer.h"
#include "../include/macros.h"

void destroy_byte_buffer(byte_buffer* bb){
  if(bb == NULL) return;

  free(bb->buffer);
  free(bb);
}

byte_buffer* new_byte_buffer(int initial){
  byte_buffer* b = alloc(byte_buffer, 1);
  b->buffer = alloc(char, initial);
  b->size = 0;
  b->capacity = initial;

  return b;
}

void append_string_to_buffer(const char* s, byte_buffer* b){
  int strl = strlen(s);
  append_bytes_to_buffer(s, b, strl);
}

void append_bytes_to_buffer(const char* bytes, byte_buffer* b, size_t size){
  if(b->size + size > b->capacity){
    b->buffer = (char*)realloc(b->buffer, (b->size+size)*2);
    b->capacity = (b->size+size)*2;
  }

  memcpy(b->buffer+b->size, bytes, size);
  b->size += size;
}

void append_buffer_to_buffer(byte_buffer* b2, byte_buffer* b){
  if(b->size + b2->size > b->capacity){
    b->buffer = (char*)realloc(b->buffer, (b->size+b2->size)*2);
    b->capacity = (b->size+b2->size)*2;
  }

  memcpy(b->buffer+b->size, b2->buffer, b2->size);
  b->size += b2->size;
}
