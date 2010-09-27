#include <stdio.h>
#include <string.h>
#include "macros.h"
#include "tree.h"
#include "list.h"
#include "node.h"

int main(){
  dom_node a, b, c, d, e, f;
  list_keeper* l = new_list(), *k;

  log(F, "Log");

  a.name = "fred";
  b.name = "fred";  
  c.name = "fred";  
  d.name = "a";  
  e.name = "vasco";  
  f.name = "rita";  

  append(l,&a);
  append(l,&b);
  append(l,&c);
  append(l,&d);
  append(l,&e);
  append(l,&f);
  
  printf("count de l: %d\n", l->count);

  k = get_by_name(*l, "fred");
  printf("count de k: %d\n", k->count);
  
  printf("%s\n", get(*k,0)->name);
  printf("%s\n", get(*k,1)->name);
  printf("%s\n", get(*k,2)->name);

  printf("O MAIS IMPORTANTE DE TODOS: %s\n", get(*l,3)->name);

  get(*k,4);

  destroy(l);
  destroy(k);

  return 0;
}
