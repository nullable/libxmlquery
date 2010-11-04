#include "../include/node.h"
#include "../include/dom_parser.h"
#include "../include/query_runner.h"
#include "../include/serialize.h"

dom_node* internal_parse_xml_file(char* filename){
    return parse_dom(filename)->root;
}

char* node_to_xml_string(dom_node* root){
    return node_to_string(root, XML);
}

int list_size(struct generic_list_s* l){
    return l->count;
}

dom_node* get_dom_node_at(struct generic_list_s* l, int p){
    return (dom_node*)get_element_at(l, p);
}

