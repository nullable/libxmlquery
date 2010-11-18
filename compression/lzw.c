#include <string.h>

#include "../include/macros.h"
#include "../include/rbtree.h"
#include "../include/bitbuffer.h"
#include "../include/byte_buffer.h"

int64_t compare_string(void* key_a, void* key_b){
    byte_buffer* ka = (byte_buffer*)key_a;
    byte_buffer* kb = (byte_buffer*)key_b;

    if(ka->size != kb->size) return kb->size - ka->size;
    else{
        int i;
        for(i = 0; i < ka->size; i++){
            if((unsigned char)ka->buffer[i] != (unsigned char)kb->buffer[i]) return (unsigned char)kb->buffer[i] - (unsigned char)ka->buffer[i];
        }
    }
    return 0;
}




bitbuffer* lzw_encode(char* byte_string, int size){
    tree_root* dict = new_rbtree(&identity_key, &compare_string);
    bitbuffer* output = new_bitbuffer(256);
    int code = 256;
    byte_buffer* prefix = alloc(byte_buffer, 1);
    prefix->capacity = (unsigned int)prefix->buffer[0];
    prefix->size = 1;
    prefix->buffer = byte_string;

    int i;
    for(i = 0; i < size; i++){
        byte_buffer* new_key = alloc(byte_buffer, 1);
        new_key->buffer = prefix->buffer;
        new_key->size = prefix->size+1;
        new_key->capacity = -1;

        byte_buffer* in_dict = (byte_buffer*)rb_tree_insert(dict, new_key);
        if(in_dict == NULL){
            //value inserted
            new_key->capacity = code++;
            append_bits_to_buffer(prefix->capacity, code/8, output);
            prefix->buffer += prefix->size;
            prefix->size = 1;
            prefix->capacity = (unsigned int)prefix->buffer[0];
        }
        else{
            //value already in
            new_key->capacity = in_dict->capacity;
            free(in_dict);
            (prefix->size)++;
            prefix->capacity = new_key->capacity;
        //    }
        }
    }
    printf("%d\n", code);
    return output;
}

char* lzw_decode(bitbuffer* bb){
    exit(1);
}

