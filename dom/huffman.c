#include "../include/huffman.h"
#include "../include/btree.h"
#include "../include/bitbuffer.h"
#include "../include/stack.h"
#include "../include/byte_buffer.h"

int compare_bc(void* o1, int16_t t1, void* o2, int16_t t2){
    return ((bc*)o2)->count - ((bc*)o1)->count;
}

void print_btree(btree* node, int depth){
    int i;
    for(i = 0; i++ < depth; printf("  "));
    if(node->left == NULL){
        printf("%c:%d\n", ((bc*)(node->value))->byte, ((bc*)(node->value))->count);
    }
    else{
        printf("| 0\n");
        print_btree(node->left, depth+1);
    }

    for(i = 0; i++ < depth; printf("  "));
    if(node->right == NULL){
        printf("%c:%d\n", ((bc*)(node->value))->byte, ((bc*)(node->value))->count);
    }
    else{
        printf("| 1\n");
        print_btree(node->right, depth+1);
    }
}



void pack_huffman_tree(btree* tree, bitbuffer* bb){
    if(tree->left != NULL && tree->right != NULL){
        append_bit_to_buffer(0, bb);
        pack_huffman_tree(tree->left, bb);
        append_bit_to_buffer(0, bb);
        pack_huffman_tree(tree->right, bb);
        append_bit_to_buffer(1, bb);
    }
    else{
        append_bit_to_buffer(1, bb);
        bc* node_bc = (bc*)tree->value;
        append_bits_to_buffer(node_bc->byte, 8, bb);
    }
}

btree* unpack_huffman_tree(bitbuffer* packed_tree, int* i){
    btree* tree = new_btree();

    char bit = get_bit_from_buffer(packed_tree, (*i)++);
    if(bit == 0){
        tree->left = unpack_huffman_tree(packed_tree, i);
        bit = get_bit_from_buffer(packed_tree, (*i)++);
        if(bit == 0) tree->right = unpack_huffman_tree(packed_tree, i);
        (*i)++; //next value must be 1
    }
    else{
        char value = get_byte_from_buffer(packed_tree, *i);
        (*i) += 8;
        tree->value = alloc(char, 1);
        *((char*)tree->value) = value;
    }

    return tree;
}

void set_translation(btree* node){
    bc* node_bc = (bc*)node->value;
    bc *left_bc, *right_bc;

    if(node->left != NULL){
        left_bc = (bc*)node->left->value;
        left_bc->bb = duplicate_bitbuffer(node_bc->bb);
        append_bit_to_buffer(0, left_bc->bb);
        set_translation(node->left);
    }

    if(node->right != NULL){
        right_bc = (bc*)node->right->value;
        right_bc->bb = duplicate_bitbuffer(node_bc->bb);
        append_bit_to_buffer(1, right_bc->bb);;
        set_translation(node->right);
    }
}

void print_simple_btree(btree* bt, int depth){
    int i;
    if(bt->left != NULL){
        for(i = 0; i++ < depth; printf("  "));
        printf("|\n");
        print_simple_btree(bt->left, depth+1);
    }
    if(bt->right != NULL){
        for(i = 0; i++ < depth; printf("  "));
        printf("|\n");
        print_simple_btree(bt->right, depth+1);
    }
    if(bt->value != NULL){
        for(i = 0; i++ < depth; printf("  "));
        printf("'%c'\n", *((char*)bt->value));
    }
}

char* huffman_decode(bitbuffer* bit_string){
    int* offset = alloc(int, 1);
    int j;
    *offset = 0;
    btree* ut = unpack_huffman_tree(bit_string, offset);

    int i = *offset;

    btree* ct = ut;
    byte_buffer* bb = new_byte_buffer(bit_string->size / 8);

    for(; i < bit_string->size; i++){
        char b = get_bit_from_buffer(bit_string, i);
        if(b == 0) ct = ct->left;
        else ct = ct->right;

        if(ct->left == NULL && ct->right == NULL){
            append_bytes_to_buffer(ct->value, bb, 1);
            ct = ut;
        }
    }

    char end = 0;
    append_bytes_to_buffer(&end, bb, 1);
    return bb->buffer;
}

bitbuffer* huffman_encode(char* byte_string, int size){
    bc* table = alloc(bc, 256);
    int i;

    //initialize
    for(i = 0; i < 256; i++){
        table[i].count = 0;
        table[i].byte = i;
        table[i].bt = NULL;
        table[i].bb = NULL;
    }

    //count
    for(i = 0; i < size; i++){
        table[(int)(byte_string[i])].count++;
    }

    //sort
    list* l = new_generic_list(256);
    for(i = 0; i < 256; i++){
        if(table[i].count == 0) continue;

        sorted_insert_element_with_type(l, table+i, 0, &compare_bc);
    }


    while(l->count > 1){
        int16_t t1 = peek_stack_type(l);
        bc* c1 = pop_stack(l);

        int16_t t2 = peek_stack_type(l);
        bc* c2 = pop_stack(l);

        bc* parent = alloc(bc, 1);
        parent->byte = 0;
        parent->count = c1->count + c2->count;
        parent->bt = new_btree();
        parent->bt->value = parent;

        if(t1 == 0){
            parent->bt->left = new_btree();
            parent->bt->left->value = c1;
        }
        else {parent->bt->left = c1->bt; }

        if(t2 == 0){
            parent->bt->right = new_btree();
            parent->bt->right->value = c2;
        }
        else {parent->bt->right = c2->bt; }

        sorted_insert_element_with_type(l, parent, 1, &compare_bc);
    }

    bc* root = pop_stack(l);
    root->bb = new_bitbuffer(256);
    set_translation(root->bt);
    bitbuffer* packed_data = new_bitbuffer(256);
    pack_huffman_tree(root->bt, packed_data);

    for(i = 0; i < size; i++){
        append_bitbuffer_to_bitbuffer(table[(int)byte_string[i]].bb, packed_data);

        int j;
        bitbuffer* bib = table[(int)byte_string[i]].bb;
    }


    return packed_data;
}

