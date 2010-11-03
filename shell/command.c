#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "symbol_table.h"
#include "command.h"
#include "../include/dom_parser.h"
#include "../include/node.h"
#include "../include/serialize.h"
#include "../include/query_runner.h"

#define COMMAND_ERROR(command_desc, reason, ...)		\
  do{								\
    printf(reason, ## __VA_ARGS__);				\
    if(!strcmp(command_desc, ""))				\
      printf("%s\n", command_desc);				\
  }while(0)

#define COMMAND(name, desc, n_args) \
  new_command(#name, desc, n_args, COMMAND_FUNCTION_NAME(name));

static command* new_command(char* name, char* desc, int n_args, int (*exec)(command* , char**)){
  command* c = alloc(command, 1);
  c->name = name;
  c->description = desc;
  c->n_args = n_args;
  c->exec = exec;
  return c;
}

static void destroy_command(command* c){
  free(c);
}

tree_root* command_table = NULL;

void* command_key(tree_node* node){
  command *c = (command*) node->node;
  return c->name;
}

int64_t command_compare(void* keyA, void* keyB){
  return strcmp( (char*) keyA, (char*) keyB);
}

void init_command_table(){
  command_table = new_rbtree(command_key, command_compare);

  command* c = COMMAND(load, "Load an xml file into a variable.\n\tUsage: load <file_to_load> <variable_name>\n", 2);
  rb_tree_insert(command_table, c);

  c = COMMAND(print_xml, "Print a loaded document into xml format\n\tUsage: print_xml <variable_name>", 1);
  rb_tree_insert(command_table, c);

  c = COMMAND(quit, "Exit from shell\n", 0);
  rb_tree_insert(command_table, c);

  c = COMMAND(help, "Print this help message\n", 0);
  rb_tree_insert(command_table, c);

  c = COMMAND(query, "Query an existing xml document\n\tUsage:query <variable_name> \"<query_expression>\"\n", 2);
  rb_tree_insert(command_table, c);
}

int exec_command(char* name, int n_args, char** args){
  command* c = (command*) search_rbtree(*command_table, name);
  if(!c){
    COMMAND_ERROR("", "Command %s not found\n", name);
    return -1;
  }
  
  if(c->n_args != n_args){
    COMMAND_ERROR(c->description, "Invalid number of arguments for command %s\n", name);
    return -1;
  }

  return c->exec(c, args);
}

COMMAND_FUNCTION_HEADER(quit){
  printf("exiting...\n");
  //TODO: destroy symbol table
  tree_iterator* it = new_tree_iterator(command_table);
  while(tree_iterator_has_next(it))
    destroy_command((command*) tree_iterator_next(it));
  destroy_iterator(it);

  exit(0);
  return 0;
}

COMMAND_FUNCTION_HEADER(query){
  doc* document = (doc*) get_symbol(argp[0]);

  if(!document){
    COMMAND_ERROR(self->description, "There is no symbol with name %s loaded\n", argp[0]);
    return 1;
  }

  list* result = query(argp[1], document->root);
  int i;
  char* str; 
  for(i = 0; i < result->count; i++){
    str = node_to_string(get_element_at(result, i), XML);
    printf("Node:\n");
    printf("%s\n", str);
    free(str);
  }
    
  return 0;
}


COMMAND_FUNCTION_HEADER(help){
  tree_iterator* it = new_tree_iterator(command_table);
  while(tree_iterator_has_next(it)){
    command *c = (command*) tree_iterator_next(it);
    printf("%s:\n", c->name);
    printf("%s", c->description);
    printf("\n");
  }
  destroy_iterator(it);
  return 0;
}

COMMAND_FUNCTION_HEADER(load){
  char *filename, *id;
  doc* document;
  
  filename = argp[0];
  document = parse_dom(filename);
  id = argp[1];
  set_symbol(id, document);

  printf("XML %s loaded and stored in %s\n", filename, id);
  return 0;
}

COMMAND_FUNCTION_HEADER(print_xml){
  doc* document = (doc*) get_symbol(argp[0]);

  if(!document){
    COMMAND_ERROR(self->description, "There is no symbol with name %s loaded\n", argp[0]);
    return 1;
  }

  char* str = node_to_string(get_doc_root(document), XML);
  printf("%s\n", str);
  free(str);
  return 0;
}
