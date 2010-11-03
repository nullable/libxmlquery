#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../dom/macros.h"
#include "symbol_table.h"
#include "command.h"

#define MAX_OPTS 20
#define LINE_SIZE 128

void print_banner(){
  printf("############################################################################\n");
  printf("#                                                                          #\n");
  printf("#  ll   iii bbb  xx   xx mm   mm ll     qqqq   uu  uu eeeee rrrr  yy   yy  #\n");
  printf("#  ll    i  bb b  xx xx  mmm mmm ll    qq  qq  uu  uu ee    rr rr  yy yy   #\n");
  printf("#  ll    i  bbb    xxx   mmmmmmm ll   qq    qq uu  uu eeee  rrrr    yyy    #\n");
  printf("#  ll    i  bb b  xx xx  mm m mm ll    qq  qq  uu  uu ee    rr rr   yyy    #\n");
  printf("#  llll iii bbb  xx   xx mm   mm llll   qqqq q uuuuuu eeeee rr  rr  yyy    #\n");
  printf("#                                                                          #\n");
  printf("############################################################################\n");
  printf("\n");
}

typedef struct parsed_line_s{
  int n_args;
  char* command;
  char* options[MAX_OPTS];
}parsed_line;

parsed_line* parse_command_line(char* line){
  parsed_line* pl = alloc(parsed_line, 1);
  char *start, *end;
  int option = 0;

  //find command
  start = end = line;
  for(; *end != ' ' && *end != '\n'; end++);
  pl->command = strndup(start, end - start);

  //find options treating double quotes
  for(; *end != '\n';){
    for(; *end == ' '; end++);

    if(*end == '"'){
      for(end++, start = end; *end != '"'; end++); 
      (pl->options)[option++] = strndup(start, end - start);
      end++; //consume double quote
      start = end;
    }else{
      for(start = end; *end != ' ' && *end != '\n'; end++); 
      (pl->options)[option++] = strndup(start, end - start);
      start = end;
    }
  }

  pl->n_args = option;
  return pl;
}

void destroy_parsed_line(parsed_line* pl){
  free(pl->command);
  int i;
  for(i = 0; i < pl->n_args; i++)
    free((pl->options)[i]);
  free(pl);
}

int main(){
  char line[LINE_SIZE] = {0};
  init_command_table();
  init_symbol_table();

  print_banner();
  while(1){
    printf(">>> ");
    fgets(line, LINE_SIZE, stdin);
    parsed_line* pl = parse_command_line(line);
    exec_command(pl->command, pl->n_args, pl->options);
    destroy_parsed_line(pl);
    memset(line, 0, LINE_SIZE);
  }
  return 0;
}
