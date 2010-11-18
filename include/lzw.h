#ifndef __LZW_H__
#define __LZW_H__


#include "../include/bitbuffer.h"

bitbuffer* lzw_encode(char* byte_string, int size);
char* lzw_decode(bitbuffer* bb);

#endif

