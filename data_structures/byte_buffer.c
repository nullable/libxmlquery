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
