#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
#include "tree.h"

tree_node NIL = {
  .node = NULL,
  .color = BLACK,
  .parent = &NIL,
  .left = &NIL,
  .right = &NIL
};

static tree_node* new_tree_node(dom_node* node){
  tree_node *z = alloc(tree_node, 1);
  
  z->node = node;
  z->parent = &NIL;
  z->left = &NIL;
  z->right = &NIL;
  z->color = RED;
  return z;
}

static tree_root* new_tree(){
  tree_root* r = alloc(tree_root, 1);
  r->root = &NIL;
  return r;
}

/*WARNNING left_rotate assumes that rotate_on->right is NOT &NIL and that root->parent IS &NIL*/
static void left_rotate(tree_root* root, tree_node* rotate_on){
  tree_node* y = rotate_on->right;
  rotate_on->right = y->left;

  if(y->left != &NIL)
    y->left->parent = rotate_on;
  
  y->parent = rotate_on->parent;
 
  if(rotate_on->parent == &NIL)
    root->root = y;
  else
    if(rotate_on == rotate_on->parent->left)
      rotate_on->parent->left = y;
    else
      rotate_on->parent->right = y;
 
  y->left = rotate_on;
  rotate_on->parent = y;
  return;
}

/*WARNNING right_rotate assumes that rotate_on->left is NOT &NIL and that root->parent IS &NIL*/
static void right_rotate(tree_root* root, tree_node* rotate_on){
  tree_node* y = rotate_on->left;
  rotate_on->left = y->right;

  if(y->right != &NIL)
    y->right->parent = rotate_on;
  
  y->parent = rotate_on->parent;
 
  if(rotate_on->parent == &NIL)
    root->root = y;
  else
    if(rotate_on == rotate_on->parent->right)
      rotate_on->parent->right = y;
    else
      rotate_on->parent->left = y;
 
  y->right = rotate_on;
  rotate_on->parent = y;
  return;
}

static void red_black_tree_insert_fixup(tree_root* root, tree_node* z){
  tree_node* y;

  while(z->parent->color == RED){
    
    if(z->parent == z->parent->parent->left){
      y = z->parent->parent->right;
      
      if(y->color == RED){
	z->parent->color = BLACK;
	y->color = BLACK;
	z->parent->parent->color = RED;
	z = z->parent->parent;
      }else{
	if(z == z->parent->right){
	  z = z->parent;
	  left_rotate(root, z);
	}
	z->parent->color = BLACK;
	z->parent->parent->color = RED;
	right_rotate(root, z->parent->parent);
      }
      
    }else{
      y = z->parent->parent->left;
      
      if(y->color == RED){
	z->parent->color = BLACK;
	y->color = BLACK;
	z->parent->parent->color = RED;
	z = z->parent->parent;
      }else{
	if(z == z->parent->left){
	  z = z->parent;
	  right_rotate(root, z);
	}
	z->parent->color = BLACK;
	z->parent->parent->color = RED;
	left_rotate(root, z->parent->parent);
      }
    }
  }

  root->root->color = BLACK;
}

void red_black_tree_insert(tree_root* root, dom_node* node){
  tree_node *y = &NIL, *x = root->root;

  tree_node *z = new_tree_node(node);

  while(x != &NIL){
    y = x;

    if(compare(key(z), key(x)) == 0){
      free(x->node);
      free(z);
      x->node = node;
      return;
    }      

    if(compare(key(z), key(x)) < 0)
      x = x->left;
    else
      x = x->right;
  }
  
  z->parent = y;

  if(y == &NIL)
    root->root = z;
  else{
    if(compare(key(z),key(y)) < 0)
      y->left = z;
    else
      y->right = z;
  }

  red_black_tree_insert_fixup(root, z);
}

dom_node* search(tree_root root, const char* name){
  tree_node *z = root.root;

  while(z != &NIL){
    if(compare(key(z), name) == 0)
      return z->node;

    if(compare(key(z), name) < 0)
      z = z->right;
    else
      z = z->left;
  }

  return NULL;
}

static void __destroy_tree(tree_node* root){
  tree_node *l, *r;

  if(root == &NIL)
    return;

  l = root->left;
  r = root->right;

  free(root);

  __destroy_tree(l);
  __destroy_tree(r);
}

void destroy_tree(tree_root* root){
  __destroy_tree(root->root);
  free(root);
}

static int match(const char *string, char *pattern, int ignore_case){
  int status;
  regex_t re;

  if(ignore_case){
    if (regcomp(&re, pattern, REG_EXTENDED | REG_ICASE | REG_NOSUB) != 0) {
      return 0;      
    }
  }else
    if (regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB) != 0) {
      return 0;      
    }

  status = regexec(&re, string, (size_t) 0, NULL, 0);
  regfree(&re);
  if (status != 0) {
    return 0;
  }
  return 1;
}

static void __regex_search(tree_node* root, char* pattern, int ignore_case, list_keeper* result){

  if(root == &NIL)
    return;

  if(match(root->node->name, pattern, ignore_case))
    append(result, root->node);

  __regex_search(root->left, pattern, ignore_case, result);
  __regex_search(root->right, pattern, ignore_case, result);
}

list_keeper* regex_search(tree_root root, char* pattern){
  list_keeper* lk = new_list();

  __regex_search(root.root, pattern, 0, lk);

  return lk;
}

list_keeper* regex_search_ignore_case(tree_root root, char* pattern){
  list_keeper* lk = new_list();

  __regex_search(root.root, pattern, 1, lk);

  return lk;
}

void dfs_print(const tree_node* root, int pad, char* pos){
  int i = 0;
  if(root == &NIL)
    return;

  for(; i < pad; i++) printf(" ");
  printf("| %s node %s ", pos, key(root));
  if(root->parent != &NIL)
    printf("with parent %s\n", key(root->parent));
  else
    printf("with no parent\n");

  dfs_print(root->left, pad + 1, "Left");
  dfs_print(root->right, pad + 1, "Right");
}

int main(){
  dom_node *one, *two, *three, *four;
  list_node *it;
  list_keeper *found;

  tree_root* r = new_tree();

  one = alloc(dom_node, 1);
  two = alloc(dom_node, 1);
  three = alloc(dom_node, 1);
  four= alloc(dom_node, 1);

  one->name = "1";
  two->name = "2";
  three->name = "3";
  four->name = "four";

  red_black_tree_insert(r,four);  
  red_black_tree_insert(r,one);
  red_black_tree_insert(r,three);  
  red_black_tree_insert(r,two);  

  dfs_print(r->root, 0, "Root");

  found = regex_search_ignore_case(*r, "[A-Z]+");

  for(it = found->first; it != NULL; it = it->next)
    printf("Found %s\n", it->node->name);

  found = regex_get_by_name_ignore_case(*found, "[A-Z]");

  for(it = found->first; it != NULL; it = it->next)
    printf("L Found %s\n", it->node->name);

  free(one);
  free(two);
  free(four);
  free(three);
  destroy_tree(r);
  return 0;
}
