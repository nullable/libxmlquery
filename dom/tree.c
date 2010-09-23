#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"
#include "node.h"

/*WARNNING left_rotate assumes that rotate_on->right is NOT NULL and that root->parent IS NULL*/
static void left_rotate(tree_node* root, tree_node* rotate_on){
  tree_node* y = rotate_on->right;
  rotate_on->right = y->left;

  if(y->left != NULL)
    y->left->parent = rotate_on;
  
  y->parent = rotate_on->parent;
 
  if(rotate_on->parent == NULL)
    root = y;
  else
    if(rotate_on == rotate_on->parent->left)
      rotate_on->parent->left = y;
    else
      rotate_on->parent->right = y;
 
  y->left = rotate_on;
  rotate_on->parent = y;
  return;
}

/*WARNNING right_rotate assumes that rotate_on->left is NOT NULL and that root->parent IS NULL*/
static void right_rotate(tree_node* root, tree_node* rotate_on){
  tree_node* y = rotate_on->left;
  rotate_on->left = y->right;

  if(y->right != NULL)
    y->right->parent = rotate_on;
  
  y->parent = rotate_on->parent;
 
  if(rotate_on->parent == NULL)
    root = y;
  else
    if(rotate_on == rotate_on->parent->right)
      rotate_on->parent->right = y;
    else
      rotate_on->parent->left = y;
 
  y->right = rotate_on;
  rotate_on->parent = y;
  return;
}

void dfs_print(const tree_node* root, int pad, char* pos){
  int i = 0;
  if(root == NULL)
    return;

  for(; i < pad; i++) printf(" ");
  printf("| %s node %s ", pos, key(root));
  if(root->parent != NULL)
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
  tree_node tone, ttwo, tthree, tfour;

  one.name = "1";
  two.name = "2";
  three.name = "3";
  four.name = "4";
  
  tone.node = &one;
  ttwo.node = &two;
  tthree.node = &three;
  tfour.node = &four;

  tone.left = NULL;
  tone.right = &ttwo;
  tone.parent = NULL;

  ttwo.left = NULL;
  ttwo.right = &tthree;
  ttwo.parent = &tone;

  tthree.left = NULL;
  tthree.right = NULL;
  tthree.parent = &ttwo;

  /*  tfour.left = NULL;
  tfour.right = NULL;
  tfour.parent = &tone;*/

  dfs_print(&tone, 0, "Root");
  left_rotate(&tone, &tone);
  printf("\nRotated left to:\n");
  dfs_print(&ttwo, 0, "Root");

  return 0;
}
