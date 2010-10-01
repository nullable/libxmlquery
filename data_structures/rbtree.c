#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "rbtree.h"

tree_node NIL = {
  .node = NULL,
  .color = BLACK,
  .parent = &NIL,
  .left = &NIL,
  .right = &NIL
};

static tree_node* new_tree_node(void* node){
  tree_node *z = alloc(tree_node, 1);
  
  z->node = node;
  z->parent = &NIL;
  z->left = &NIL;
  z->right = &NIL;
  z->color = RED;
  return z;
}

tree_root* new_tree(void* (*key_function_pointer)(struct stree_node* node),
		    int (*compare_function_pointer)(void* keyA, void* keyB)){
  tree_root* r = alloc(tree_root, 1);
  r->root = &NIL;
  r->key = key_function_pointer;
  r->compare = compare_function_pointer;
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

void red_black_tree_insert(tree_root* root, void* node){
  tree_node *y = &NIL, *x = root->root;

  tree_node *z = new_tree_node(node);

  while(x != &NIL){
    y = x;

    if(root->compare(root->key(z), root->key(x)) == 0){
      free(z);
      x->node = node;
      return;
    }      

    if(root->compare(root->key(z), root->key(x)) < 0)
      x = x->left;
    else
      x = x->right;
  }
  
  z->parent = y;

  if(y == &NIL)
    root->root = z;
  else{
    if(root->compare(root->key(z),root->key(y)) < 0)
      y->left = z;
    else
      y->right = z;
  }

  red_black_tree_insert_fixup(root, z);
}

static tree_node* __search_tree_node(tree_root root, void* key){
  tree_node *z = root.root;

  while(z != &NIL){
    if(root.compare(root.key(z), key) == 0)
      return z;

    if(root.compare(root.key(z), key) < 0)
      z = z->right;
    else
      z = z->left;
  }

  return NULL;
}

void* search(tree_root root, void* key){
  tree_node *z = __search_tree_node(root, key);
  if(z)
    return z->node;
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

static void __red_black_transplant(tree_root* root, tree_node* u, tree_node* v){
  if(u->parent == &NIL)
    root->root = v;
  else
    if(u == u->parent->left)
      u->parent->left = v;
    else
      u->parent->right = v;
  v->parent = u->parent;
}

static tree_node* __red_black_tree_minimum(tree_node* z){
  for(; z->left != &NIL; z = z->left);
  return z;
}

static void __red_black_tree_delete_fixup(tree_root* root, tree_node* x){
  tree_node* w;
  while( x != root->root && x->color == BLACK){
    if(x == x->parent->left){
      w = x->parent->right;
      if(w->color == RED){
	w->color = BLACK;
	x->parent->color = RED;
	left_rotate(root, x->parent);
	w = x->parent->right;
      }
      if(w->left->color == BLACK && w->right->color == BLACK){
	w->color = RED;
	x = x->parent;
      }
      else{
	if(w->right->color == BLACK){
	  w->left->color = BLACK;
	  w->color = RED;
	  right_rotate(root, w);
	  w = w->parent->right;
	}
	w->color = x->parent->color;
	x->parent->color = BLACK;
	w->right->color = BLACK;
	left_rotate(root, x->parent);
	x = root->root;
      }
    }
    else{
      w = x->parent->left;
      if(w->color == RED){
	w->color = BLACK;
	x->parent->color = RED;
	right_rotate(root, x->parent);
	w = x->parent->left;
      }
      if(w->right->color == BLACK && w->left->color == BLACK){
	w->color = RED;
	x = x->parent;
      }
      else{
	if(w->left->color == BLACK){
	  w->right->color = BLACK;
	  w->color = RED;
	  left_rotate(root, w);
	  w = w->parent->left;
	}
	w->color = x->parent->color;
	x->parent->color = BLACK;
	w->left->color = BLACK;
	right_rotate(root, x->parent);
	x = root->root;
      }
    }
  }
  x->color = BLACK;
  return;
}

void red_black_tree_delete(tree_root* root, void* key){
  tree_node *y, *z, *x;
  int y_original_color;

  y = z = __search_tree_node(*root, key);

  if(y == NULL){
    log(W, "Trying to remove a node from tree that does not exist.");
    return;
  }

  y_original_color = y->color;
  if(z->left == &NIL){
    x = z->right;
    __red_black_transplant(root, z, z->right);
  }
  else
    if(z->right == &NIL){
      x = z->left;
      __red_black_transplant(root, z, z->left);
    }
    else{
      y = __red_black_tree_minimum(z->right);
      y_original_color = y->color;
      x = y->right;
      if(y->parent == z)
	x->parent = y;
      else{
	__red_black_transplant(root, y, y->right);
	y->right = z->right;
	y->right->parent = y;
      }
      __red_black_transplant(root, z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }
  if(y_original_color == BLACK)
    __red_black_tree_delete_fixup(root, x);
}

void print(tree_node* root, int pad){
  int i;

  if(root == &NIL)
    return;

  printf("|");
  for(i = 0; i < pad; i++, printf("--"));
  if(root->parent != &NIL)
    printf("+ node %p with parent %p\n", root->node, root->parent->node);
  else
    printf("+ node %p with no parent\n", root->node);
  printf("|\n");

  print(root->left, pad + 1);
  print(root->right, pad + 1);
}
