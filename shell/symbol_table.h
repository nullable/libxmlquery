#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

typedef struct symbol_s{
  char* id;
  void* value;
}symbol;

extern void* get_symbol(char* id);
extern void set_symbol(char* id, void* value);
extern void init_symbol_table();
#endif
