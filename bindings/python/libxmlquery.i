
%module libxmlquery
%{
/* Put headers and other declarations here */
#include <xmlquery/node.h>
#include <xmlquery/stack.h>

extern dom_node* internal_parse_xml_file(char* filename);
extern char* node_to_xml_string(dom_node* root);
extern int list_size(struct generic_list_s* l);
extern struct generic_list_s* query(char* q, dom_node*);
extern dom_node* get_dom_node_at(struct generic_list_s* l, int p);
%}

extern void set_namespace(dom_node* node, char* namespace);
extern void set_name(dom_node* node, char* name);
extern void set_value(dom_node* node, char* value);
extern void set_parent(dom_node* node, dom_node* parent);

extern char* get_namespace(dom_node* node);
extern char* get_name(dom_node* node);
extern char* get_value(dom_node* node);

extern void prepend_child(dom_node* parent, dom_node* child);
extern void append_child(dom_node* parent, dom_node* child);
extern void append_children(dom_node* parent, struct generic_list_s* children);
extern void add_attribute(dom_node* node, dom_node* attribute);

extern dom_node* new_element_node(char* name);
extern dom_node* new_text_node(char* text);
extern dom_node* new_attribute(char* name, char* value);
extern dom_node* new_cdata(char* cdata_text);

extern dom_node* get_child_at(dom_node* parent, int index);
extern struct generic_list_s* get_children(dom_node* node);
extern dom_node* get_attribute_by_name(dom_node* node, char* attr_name);

extern dom_node* delete_attribute(dom_node* node, char* name);

//query_runner
extern struct generic_list_s* query(char* q, dom_node*);

//helpers
extern dom_node* internal_parse_xml_file(char* filename);
extern char* node_to_xml_string(dom_node* root);
extern int list_size(struct generic_list_s* l);
extern dom_node* get_dom_node_at(struct generic_list_s* l, int p);

//stack.h
extern void remove_element(struct generic_list_s* l, void* obj);

