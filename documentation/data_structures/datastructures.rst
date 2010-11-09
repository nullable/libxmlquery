.. highlight:: c

===============
Data Structures
===============

Generic List
------------

.. c:type:: generic_list

Generic array list optimized for fast access.

List
^^^^

Stack
^^^^^

Queue
^^^^^

Generic Red Black Tree
----------------------
The file rbtree.c contains the source code for a generic red black tree. This data structure is capable of storing any kind of data and its defined in rbtree.h as:

.. c:type:: tree_root
.. c:type:: struct sroot

::

    typedef struct sroot{
       struct stree_node* root;
       void* (*key)(struct stree_node* node);
       int64_t (*compare)(void* keyA, void* keyB);
    }tree_root;

There are two things to notice here. First, this is only the root of the tree and points to the first tree_node of the rbtree. Second, it contains two function pointers. The first points to a function, which receives a :c:type:`tree_node` and should return a **pointer** to that node's key. The second is a pointer to a compare function, which compares two keys and should return:

- A negative integer if the first key is smaller than the second.
- 0 if the keys are the same.
- A positive integer if the first key is bigger than the second.

These pointers must be provided by the user. Why do we need these pointers? Because the data stored in the rbtree can be anything, but we still need to know how to sort it. Nevertheless, if you wish to use this data structure as a container and don't care how things are sorted, you can always use the method :c:func:`new_simple_rbtree`.

Each node in an rbtree is called a tree_node and is defined in rbtree.h as:

.. c:type:: tree_node
.. c:type:: struct stree_node

::

     typedef struct stree_node{
        void* node;

	uint8_t color;
  
        struct stree_node* parent;
  	struct stree_node* left;
  	struct stree_node* right;
      }tree_node;

There's not much to say about this structure, the only thing relevant is the field ``node``, which is used to store the actual data. The other fields are used to keep the rbtree intact.

Finally, there's one more structure, which is defined in rbtree.h as:

.. c:type:: tree_iterator
.. c:type:: struct siterator

::

      typedef struct siterator{
         struct stree_node* current;
      }tree_iterator;

As the name implies, this structure is an iterator to the tree nodes.

As a final note, remember that we provide functions to destroy our structures, but the actual data must be destroyed by you. Do not use iterators for this purpose.

Function description
^^^^^^^^^^^^^^^^^^^^

.. c:function:: tree_root* new_simple_rbtree()

   This function creates an rbtree, which sorts the data acording to its memory pointer. This function should be used when you just want the rbtree to behave as a container,
   but you still need O(log(n)) when accessing the data. Keep in mind that in order to retreive the stored data, you need to know it's memory pointer.
   
   The return value is a tree_root structure.

.. c:function:: tree_root* new_rbtree(void* (*key_function_pointer)(struct stree_node* node), int64_t (*compare_function_pointer)(void* keyA, void* keyB))

   :c:member:`key_function_pointer` A function that should return the address of the node's key.

   :c:member:`compare_function_pointer` A function that should compare two keys and return values as described above. It receives the addresses of each key.

   This function creates an rbtree, which sorts the data according to the given functions. The following example shows how to create an rbtree to store integers.

   Example::

     #include <stdio.h>
     #include "rbtree.h"

     void* key_address(tree_node* node){
        return node->node;
     }

     int64_t compare_integers(void* keyA, void* keyB){
        return *((int *) keyA) - *((int *) keyB);
     }

     int main(){
        tree_root* rbtree = new_rbtree(& key_address, & compare_integers);
	return 0;
     }

   You may compile it with

   .. code-block:: bash 

     gcc -o test <above_source_file> -I<folder path where rbtree.h is kept>

.. c:function:: void* rb_tree_insert(tree_root* root, void* node)

   :c:member:`root` A pointer to the tree root where to insert the data represented by ``node``.

   :c:member:`node` A pointer to the data, which will be inserted in the tree.

   As the name implies this function inserts data into the rbtree. In the eventuality that the inserted value is already in the tree, it will be replaced and a pointer to the older value is returned. This is done so the user can free the space stored by that data. The following example shows how to insert integers in an rbtree.

   Example::

     #include <stdio.h>
     #include "rbtree.h"

     void* key_address(tree_node* node){
        return node->node;
     }

     int64_t compare_integers(void* keyA, void* keyB){
        return *((int *) keyA) - *((int *) keyB);
     }

     int main(){
        tree_root* rbtree = new_rbtree(& key_address, & compare_integers);
	int a = 9, b = 6, c = 10, d = 6;
	
	rb_tree_insert(rbtree, &a);
        rb_tree_insert(rbtree, &b);
	rb_tree_insert(rbtree, &c);
	int older = *((int *) rb_tree_insert(rbtree, &d));
	
	//don't free older because it was "alloched" by the compiler.
	printf("Found a %d already stored in the tree.\n", older);

	return 0;
     }

   You may compile it with

   .. code-block:: bash 

     gcc -o test <above_source_file> -I<folder path where rbtree.h is kept>

.. c:function:: void* rb_tree_delete(tree_root* root, void* key)

   :c:member:`root` A pointer to the tree root where to delete the data with key pointed by ``key``.

   :c:member:`key` A pointer to the key of the node to be deleted.

   This function deletes a node from an rbtree. If a node with a key equal to the one pointed by ``key`` does not exist, NULL will be return. However, if such a node is found, then a pointer to the data is returned. This is done so the user can free the space used by that data. The following example shows how to use this function on an rbtree that stores integers.

   Example::

     #include <stdio.h>
     #include "rbtree.h"

     void* key_address(tree_node* node){
        return node->node;
     }

     int64_t compare_integers(void* keyA, void* keyB){
        return *((int *) keyA) - *((int *) keyB);
     }

     int main(){
        tree_root* rbtree = new_rbtree(& key_address, & compare_integers);
	int a = 9, b = 6, c = 10;
	
	rb_tree_insert(rbtree, &a);
        rb_tree_insert(rbtree, &b);
	rb_tree_insert(rbtree, &c);
	
	int d = 10, stored;
	stored = *((int *) rb_tree_delete(rbtree, &d));
	
	//don't free stored because it was "alloched" by the compiler.
	printf("Found a %d stored in the tree.\n", stored);		

	return 0;
     }

   You may compile it with

   .. code-block:: bash 

     gcc -o test <above_source_file> -I<folder path where rbtree.h is kept>



.. c:function:: void* search_rbtree(tree_root root, void* key)

   :c:member:`root` The root of the tree where to perform the search.

   :c:member:`key` A pointer to the key of the node to be searched.

   This function searchs an rbtree for a node. It returns NULL if nothing is found, or the data stored in the tree with a key equal to the value pointed by ``key``. The following example shows how to search a tree that stores integers.

   Example::

     #include <stdio.h>
     #include "rbtree.h"

     void* key_address(tree_node* node){
        return node->node;
     }

     int64_t compare_integers(void* keyA, void* keyB){
        return *((int *) keyA) - *((int *) keyB);
     }

     int main(){
        tree_root* rbtree = new_rbtree(& key_address, & compare_integers);
	int a = 9, b = 6, c = 10;
	
	rb_tree_insert(rbtree, &a);
        rb_tree_insert(rbtree, &b);
	rb_tree_insert(rbtree, &c);
	
	int d = 10, stored;
	stored = *((int *) search_rbtree(*rbtree, &d));
	
	printf("Found a %d stored in the tree.\n", stored);		

	return 0;
     }

   You may compile it with

   .. code-block:: bash 

     gcc -o test <above_source_file> -I<folder path where rbtree.h is kept>

.. c:function:: void destroy_rbtree(tree_root* root)

   :c:member:`root` A pointer to the tree to be destroyed.

   This function destroys an rbtree. Note that this doesn't free the user stored data. The following example shows how to use this in a tree that stores integers.

   Example::

     #include <stdio.h>
     #include "rbtree.h"

     void* key_address(tree_node* node){
        return node->node;
     }

     int64_t compare_integers(void* keyA, void* keyB){
        return *((int *) keyA) - *((int *) keyB);
     }

     int main(){
        tree_root* rbtree = new_rbtree(& key_address, & compare_integers);
	int a = 9, b = 6, c = 10;
	
	rb_tree_insert(rbtree, &a);
        rb_tree_insert(rbtree, &b);
	rb_tree_insert(rbtree, &c);
	
	destroy_rbtree(rbtree);
	//We do not need to free the stored data because it was "alloched" by the compiler.

	return 0;
     }

   You may compile it with 

   .. code-block:: bash 

     gcc -o test <above_source_file> -I<folder path where rbtree.h is kept>

   Notice that running the command 

   .. code-block:: bash 

      valgrind --show-reachable=yes --leak-check=full ./test

   produces the ouput::

      ==1188== HEAP SUMMARY:
      ==1188==     in use at exit: 0 bytes in 0 blocks
      ==1188==   total heap usage: 4 allocs, 4 frees, 72 bytes allocated
      ==1188== 
      ==1188== All heap blocks were freed -- no leaks are possible

   Which means that there are no memory leaks and you should always use this function to free the space store by any rbtree you use.

.. c:function:: tree_iterator* new_tree_iterator(tree_root* root)

   :c:member:`root` A pointer to a tree root, which the iteration will be performed.

   This function creates an iterator to an rbtree. Note that when you create an iterator, you should not insert or delete nodes from the tree before the iteration is over. Otherwise, the behaviour of the program will be unpredictable. It returns pointer to the created iterator. The following example shows how to create an iterator for a tree that stores integers.

   Example::

     #include <stdio.h>
     #include "rbtree.h"

     void* key_address(tree_node* node){
        return node->node;
     }

     int64_t compare_integers(void* keyA, void* keyB){
        return *((int *) keyA) - *((int *) keyB);
     }

     int main(){
        tree_root* rbtree = new_rbtree(& key_address, & compare_integers);
	int a = 9, b = 6, c = 10;
	
	rb_tree_insert(rbtree, &a);
        rb_tree_insert(rbtree, &b);
	rb_tree_insert(rbtree, &c);
	
	tree_iterator* it = new_tree_iterator(rbtree);

	return 0;
     }

   You may compile it with 

   .. code-block:: bash 

     gcc -o test <above_source_file> -I<folder path where rbtree.h is kept>

.. c:function:: uint8_t tree_iterator_has_next(tree_iterator* it)
.. c:function:: void* tree_iterator_next(tree_iterator* it)
.. c:function:: void destroy_iterator(tree_iterator* it)
