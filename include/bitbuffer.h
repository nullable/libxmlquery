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

