#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"
#include "node.h"
#include "macros.h"

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
    
    root->root->color = BLACK;
  }
}

void red_black_tree_insert(tree_root* root, dom_node* node){
  tree_node *y = &NIL, *x = root->root;

  tree_node *z = new_tree_node(node);

  while(x != &NIL){
    y = x;
    if(compare(z, x) < 0)
      x = x->left;
    else
      x = x->right;
  }
  
  z->parent = y;

  if(y == &NIL)
    root->root = z;
  else{
    if(compare(z,y) < 0)
      y->left = z;
    else
      y->right = z;
  }

  red_black_tree_insert_fixup(root, z);
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
  for(i = 0; i < pad; i++) printf(" ");
  printf("\t+------\n");

  dfs_print(root->left, pad + 1, "Left");
  dfs_print(root->right, pad + 1, "Right");
}

int main(){
  dom_node one, two, three, four;
  tree_root* r = new_tree();

  one.name = "1";
  two.name = "2";
  three.name = "3";
  four.name = "4";

  red_black_tree_insert(r,&one);
  red_black_tree_insert(r,&two);
  
  dfs_print(r->root, 0, "Root");
  return 0;
}
