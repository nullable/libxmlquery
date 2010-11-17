#include "../include/huffman.h"
#include "../include/btree.h"
#include "../include/bitbuffer.h"
#include "../include/stack.h"

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

void set_translation(btree* node){
    bc* node_bc = (bc*)node->value;
    bc *left_bc, *right_bc;
    int j;

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
    else{
        printf("%d:%d => ", node_bc->byte, node_bc->count);
        for(j = 0; j < node_bc->bb->size; j++){
            printf("%d", get_bit_from_buffer(node_bc->bb, j));
        }
        printf("\n");
    }
}

btree* get_huffman_tree(char* byte_string, int size){
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

        sorted_insert_element_with_type_at(l, table+i, 0, &compare_bc);
    }

    for(i = 0; i < l->count; i++){
        printf("%d:%d <> ", ((bc*)get_element_at(l, i))->byte, ((bc*)get_element_at(l, i))->count);
    }
    printf("\n");

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

        sorted_insert_element_with_type_at(l, parent, 1, &compare_bc);
    }

    bc* root = pop_stack(l);
    print_btree(root->bt, 1);

    //root->bt->value = root;
    root->bb = new_bitbuffer(256);
    set_translation(root->bt);
/*
    for(i = 0; i < 256; i++){
        if(table[i].count == 0) continue;
        printf("%d:%d => ", table[i].byte, table[i].count);

        int j;
        bitbuffer* bb = table[i].bb;
        for(j = 0; j < bb->size; j++){
            printf("%d", get_bit_from_buffer(bb, j));
        }
        printf("\n");
    }
*/
    return NULL;

}

