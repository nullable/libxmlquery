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

#include <stdio.h>
#include <string.h>
#include "include/node.h"
#include "include/serialize.h"
#include "include/lxq_parser.h"
#include "include/stack.h"
#include "include/query_runner.h"
#include "include/node.h"

int main(int argc, char** argv){
    int i;
    char *filename, *query_string, *node;

    if(argc == 2){
        filename = "-";
    }
    else if(argc != 3){
        printf("usage: %s \"query\" \"xml_file\"\n", argv[0]);
        return 0;
    }
    else{
        filename = argv[2];
    }

    query_string = argv[1];

    doc* document = parse_xml(filename);
    if(document == NULL) exit(1);

    list* result = query(query_string, document->root);

    if(!result) exit(1);

    for(i=0; i < result->count; i++){
        dom_node* t = (dom_node*)get_element_at(result, i);
        node = node_to_string(t, XML);
        printf("%s", node);
        free(node);
    }

    if(result)
    destroy_generic_list(result);

    if(document != NULL)
    destroy_dom_tree(document);

    destroy_dictionary();
    destroy_custom_filters();
    return 0;
}

