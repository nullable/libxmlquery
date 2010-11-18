#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <stdarg.h>
#include "../include/macros.h"
#include "../include/stack.h"
#include "../include/rbtree.h"

typedef struct command_s{
  char* name;
  char* description;
  int n_args;
  int (*exec)(struct command_s* self, char** args);
} command;

#define COMMAND_FUNCTION_NAME(command_name) command_name ## _exec

#define COMMAND_FUNCTION_HEADER(command_name) int COMMAND_FUNCTION_NAME(command_name)(struct command_s* self, char** argp)

extern COMMAND_FUNCTION_HEADER(load);
extern COMMAND_FUNCTION_HEADER(print_xml);
extern COMMAND_FUNCTION_HEADER(quit);
extern COMMAND_FUNCTION_HEADER(help);
extern COMMAND_FUNCTION_HEADER(query);
extern COMMAND_FUNCTION_HEADER(gdb);

extern void init_command_table();
extern int exec_command(char* name, int n_args, char** args);
#endif
