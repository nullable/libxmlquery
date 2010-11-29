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
