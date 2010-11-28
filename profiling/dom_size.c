#include <stdio.h>
#include <string.h>
#include "../include/node.h"
#include "../include/lxq_parser.h"
#include "../include/stack.h"
#include "../include/query_runner.h"
#include "../include/node.h"

void get_size(dom_node* n, int *size){
    int i;
    *size += sizeof(n);
    switch(n->type){
        case CDATA:
        case TEXT_NODE:
            *size += strlen(n->value)*sizeof(char);
            break;
        case ATTRIBUTE:
            *size += strlen(n->name)*sizeof(char);
            *size += strlen(n->value)*sizeof(char);
            break;
        case ELEMENT:
            if(n->children != NULL)
              for(i = 0; i < n->children->count; i++){ get_size(get_element_at(n->children, i), size); }

            if(n->attributes != NULL){
                struct siterator *it = new_tree_iterator(n->attributes);
                while(tree_iterator_has_next(it)){ get_size(tree_iterator_next(it), size); }
                destroy_iterator(it);
            }
            *size += strlen(n->name)*sizeof(char);
            if(n->namespace != NULL) *size += strlen(n->namespace)*sizeof(char);
            break;
    }
}

int main(int argc, char** argv){
    if(argc != 2){
        printf("usage: %s \"xml_file\"\n", argv[0]);
        return 0;
    }

    doc* document = parse_xml(argv[1]);
    int i = 0;
    get_size(document->root, &i);
    printf("%d kB\n", i / 1024);
}

