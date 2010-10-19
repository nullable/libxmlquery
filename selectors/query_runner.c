extern list* get_xml_descendants(dom_node*);
extern list* get_xml_children(dom_node*);
extern list* get_xml_nodes_after(dom_node*);
extern list* remove_duplicates(list*)
extern dom_node* get_xml_node_after(dom_node*);


list* apply_operator(list* nodes, int op){
    list* result = new_list(1);
    for(int i = 0; i < nodes->count; i++){
        dom_node* node = get_element_at(nodes, i);
        switch(op){
        case SPACE:
            result = merge_lists(result, get_xml_descendants(node));
            break;
        case '>':
            result = merge_lists(result, get_xml_children(node));
            break;
        case '~':
            result = merge_lists(result, get_xml_nodes_after(node));
            break;
        case '+':
            append_element(result, get_xml_node_after(node));
            break;
        }
    }

    return remove_duplicates(result);
}



list* query(char* query_string, dom_node* node){
    list* all_nodes = get_descendants(node);
    append_element(all_nodes, node);

    list* nodes = all_nodes;
    list* result = new_list(1);

    queue* query = parse_query(query_string);
    while(query->count > 0){
        switch(peek_queue_type(query)){
        case LXQ_RELATION_TYPE:
            int op = (int)dequeue(query);
            if(op == ','){
                result = merge_lists(result, nodes);
                nodes = all_nodes;
            }
            else{
                list* old_nodes = nodes;
                nodes = apply_operator(nodes, op);
                destroy_generic_list(old_nodes);
            }
            break;
        case LXQ_SELECTOR_TYPE:
            selector* s =(selector*)dequeue(query);
            filter_nodes(nodes, s);
            break;
        }
    }

    return remove(duplicates, merge_lists(result, nodes));
}

