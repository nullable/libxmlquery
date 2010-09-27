#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "eregex.h"
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

tree_root* new_tree(){
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

static void __append_node_to_iterator(tree_iterator* it, tree_node* node){
  iterator_node* n;

  if(node == NULL)
    return;

  n = alloc(iterator_node, 1);

  n->node = node;
  n->next = NULL;

  if(it->first == NULL){
    it->first = n;
    it->last = n;
    return;
  }

  it->last->next = n;
  it->last = n;
}

tree_iterator* new_tree_iterator(tree_root* root){
  tree_iterator* it = alloc(tree_iterator, 1);
  it->first = NULL;
  it->last = NULL;
  __append_node_to_iterator(it, root->root);
  return it;
}

int tree_iterator_has_next(tree_iterator* it){
  if(it->first == NULL)
    return 0;
  return 1;
}

struct snode* tree_iterator_next(tree_iterator* it){
  iterator_node *aux;
  dom_node* res;

  if(it->first == NULL){
    log(E,"Trying to retrieve the next element of an empty iterator.\n");
    return NULL;
  }

  aux = it->first;
  it->first = it->first->next;
  res = aux->node->node;

  __append_node_to_iterator(it, aux->node->left);
  __append_node_to_iterator(it, aux->node->right);

  free(aux);
  return res;
}

void destroy_iterator(tree_iterator* it){
  iterator_node* aux;

  aux = it->first;
  while(aux != NULL){
    it->first = it->first->next;
    free(aux);
    aux = it->first;
  }
  free(it);
}
