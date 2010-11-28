.. highlight:: c

=====================
Document Object Model
=====================

DOM
---

After the parsing of the xml is done, a DOM structure is created and
can be accessed through a global variable denominated ``lxq_document``.
Bear in mind that this variable is dependent of the current parser.
In other words, if the parser module changes, then the variable name
may also change.

The DOM structure is a tree, where each node is an xml element. Since
the input xml has already been validated we know it has only one root
, so the document variable will point to a single node in the dom
tree, the root node (This can be accessed through document->root, but we really recommend the use of :c:func:`get_doc_root`).

The dom structure provides flexible and extendable xml document
handling. It has the capability to add, remove and modify elements in
the xml. 

Data types
^^^^^^^^^^
.. c:type:: dom_node
.. c:type:: struct snode

The structure definition for a dom node is in the file node.h.

::

  typedef struct snode{
    enum node_type type;

    char* namespace;
    char* name;
    char* value;

    struct sroot* attributes;

    struct snode* parent;

    struct generic_list_s* children;
  }dom_node;

This structure is used to store any kind of entity in an xml file. It can be an element, attribute, cdata or text node. In order to control these different kind of entities, the :c:type:`dom_node` has an enumerate field. This field is called :c:type:`node_type` .

.. c:type:: node_type

This enum is defined as::

  enum node_type {
    ATTRIBUTE,
    ELEMENT,
    TEXT_NODE,
    CDATA
  };

Each enum represents a different kind of xml entity.

.. c:type:: doc
.. c:type:: struct sdoc

Finally, a document is a structure that points to two dom nodes - The root of the dom tree and the root of the xml declaration tree. Lets look at its definition.

::

  typedef struct sdoc{
    struct snode* root;
    struct snode* xml_declaration;
  }doc;

Suppose we have the following xml file

.. code-block:: xml

   <?xml version="1.0" ?>
   <x>
     ...
   </x>

When this xml is parsed, a :c:type:`doc` is created with the field ``xml_declaration`` pointing to a tree that parsed '<?xml version="1.0" ?>' and the ``root`` field pointing to a tree that parsed the rest of the document.

Function description
^^^^^^^^^^^^^^^^^^^^
.. c:function:: char* set_namespace(dom_node* node, char* namespace)

   :c:member:`node` The node whose namespace will be set.

   :c:member:`namespace` The namespace to set.

   This function sets the namespace of the given node. If the node already contained a namespace, a pointer to it will be returned. Otherwise, NULL is returned.

.. c:function:: char* set_name(dom_node* node, char* name)

   :c:member:`node` The node whose name will be set.

   :c:member:`name` The name to set.

   This function sets the name of the given node. If the node already contained a name, a pointer to it will be returned. Otherwise, NULL is returned.

.. c:function:: char* set_value(dom_node* node, char* value)

   :c:member:`node` The node whose value will be set.

   :c:member:`value` The value to set.

   This function sets the value of the given node. If the node already contained a value, a pointer to it will be returned. Otherwise, NULL is returned.

.. c:function:: dom_node* set_doc_root(doc* document, struct snode* root)

   :c:member:`doc` The document whose root will be set.

   :c:member:`root` The root to set.

   This function sets the root of the given document. If the document already contained a root, a pointer to it will be returned. Otherwise, NULL is returned.

.. c:function:: dom_node* set_xml_declaration(doc* document, struct snode* vers)

   :c:member:`doc` The document whose xml declaration will be set.

   :c:member:`vers` The xml declaration to set.

   This function sets the xml declaration of the given document. If the document already contained an xml declaration, a pointer to it will be returned. Otherwise, NULL is returned.

.. c:function:: void set_parent(dom_node* node, dom_node* parent)

   :c:member:`node` The node whose parent will be set.

   :c:member:`parent` The parent to set.

   This function sets the parent of the given node. If the node already contained a parent, a pointer to it will be returned. Otherwise, NULL is returned.

.. c:function:: char* get_namespace(dom_node* node)

   :c:member:`node` The node from which the namespace will be returned.

   This functions returns the namespace of the given node, or NULL if the node doesn't contain any.

.. c:function:: char* get_name(dom_node* node)

   :c:member:`node` The node from which the names will be returned.

   This functions returns the names of the given node, or NULL if the node doesn't contain any.

.. c:function:: char* get_value(dom_node* node)

   :c:member:`node` The node from which the value will be returned.

   This functions returns the value of the given node, or NULL if the node doesn't contain any.

.. c:function:: dom_node* get_xml_declaration(doc* document)

   :c:member:`document` The document from which the xml declaration tree will be returned.

   This functions returns the dom node at the root of the xml declaration tree, or NULL if there isn't any.

.. c:function:: dom_node* get_doc_root(doc* document)

   :c:member:`document` The document from which the dom tree will be returned.

   This functions returns the dom node at the root of the dom tree, or NULL if there isn't any.

.. c:function:: void prepend_child(dom_node* parent, dom_node* child)

   :c:member:`parent` The parent node to whom the child will be prepended.

   :c:member:`child` The child to be prepended.

   This function inserts a dom node at the head of the children's list of the dom node given as first argument.

.. c:function:: void append_child(dom_node* parent, dom_node* child)

   :c:member:`parent` The parent node to whom the child will be appended.

   :c:member:`child` The child to be appended.

   This function inserts a dom node at the end of the children's list of the dom node given as first argument.

.. c:function:: void append_children(dom_node* parent, struct generic_list_s* children)

   :c:member:`parent` The parent node to whom the children list will be appended.

   :c:member:`children` The children list to be appended.

   This functions calls :c:func:`append_child` for each node in the children's list, passed as the second argument.

.. c:function:: void add_attribute(dom_node* node, dom_node* attribute)

   :c:member:`node` The node to whom the attribute will be added.

   :c:member:`attribute` The attribute to add to the node.

   This function adds an attribute to the node given as the first argument.

.. c:function:: doc* new_document(struct snode* xml_declaration)

   :c:member:`xml_declaration` The declaration tree of the document. Can be NULL.

   This function creates a new document with the given xml declaration.

.. c:function:: dom_node* new_element_node(char* name)

   :c:member:`name` The name of the new element node.

   This function creates a new element node with the given name. The name should correspond to an xml tag.

.. c:function:: dom_node* new_text_node(char* text)

   :c:member:`text` The text in the xml document.

   This function creates a new text node and stores the given text in the value field of the dom node.

.. c:function:: dom_node* new_attribute(char* name, char* value)

   :c:member:`name` The name of the attribute.

   :c:member:`value` The value of the attribute.

   This function creates a new attribute with the given name and value.

.. c:function:: dom_node* new_cdata(char* cdata_text)

   :c:member:`cdata_text` The cdata text in the xml document.

   This function creates a new cdata node and stores the text in the value field.

.. c:function:: dom_node* get_child_at(dom_node* parent, int index)

   :c:member:`parent` The node from which the child will be returned.

   :c:member:`index` The index of the child beggining at 0.

   This function returns the nth child of the parent node. If no child is found, NULL will be returned.

.. c:function:: struct generic_list_s* get_children(dom_node* node)

   :c:member:`node` The node from which the children will be returned.

   This function returns a pointer to the node's children. Be aware that this function only returns nodes directly below the given argument. If you want the other ones too, consider using :c:func:`get_descedants`.

.. c:function:: struct generic_list_s* get_descendants(dom_node* node)

   :c:member:`node` The node from which the descedants will be returned.

   This function returns the descendants of the node. This differs from :c:func:`get_children` because it returns all nodes below the given argument. 

.. c:function:: struct generic_list_s* get_text_nodes(doc* root)

   :c:member:`root` The document root of the xml.

   This function traverses the entire dom tree and returns all text nodes in it.

.. c:function:: dom_node* get_attribute_by_name(dom_node* node, char* attr_name)

   :c:member:`node` The node from whom the attribute will be returned.

   :c:member:`attr_name` The name of the attribute.

   This function returns the attribute with the given name, or NULL if there isn't any.

.. c:function:: struct generic_list_s* get_elements_by_name(doc* root, char* name)

   :c:member:`root` The document root of the xml.

   :c:member:`name` The name of the elements to search.

   This function traverses the entire dom tree and returns all element with the given name.

.. c:function:: dom_node* delete_attribute(dom_node* node, char* name)

   :c:member:`node` The node from whom the attribute will be removed.

   :c:member:`name` The name of the attribute to remove.

   This function deletes the attribute with the given name from the given node. If the node doesn't contain an attribute with the given name, then nothing will happen.

.. c:function:: void remove_node(doc* root, dom_node* node)

   :c:member:`root` The root of the xml document.

   :c:member:`node` The node to remove from the dom tree.

   This function removes the given node from the given dom tree. Be carefull because it also removes all nodes below the given node. This doesn't free the given node, so you should keep the pointer to it and free it yourself.

.. c:function:: void destroy_dom_node(dom_node* n)

   :c:member:`n` The dom node to be destroyed.

   This function should be used with caution. This frees the space associated with the given dom node, but it also frees all nodes below it. A simple way to clean the dom tree keeping only the document structure is to pass the root node to this function as follows::

     #include "node.h"
     #include "lxq_parser.h"   
   
     int main(){
       doc* document = parse_dom("xml_file.xml");
       //clean the tree but keep my doc structure and xml declaration intact.
       destroy_dom_node(get_doc_root(document));
       return 0;
     }

   You may compile it with

   .. code-block:: bash 

     gcc -o test <above_source_file> -I<folder path where node.h and lxq_parser.h are kept>

.. c:function:: void destroy_dom_tree(doc* root)

   :c:member:`root` The document to be destroyed.

   This function does exactly the same thing as :c:func:`destroy_dom_node` but it also frees the space associated with the document structure. As an example consider::

     #include "node.h"
     #include "lxq_parser.h"   
   
     int main(){
       doc* document = parse_dom("xml_file.xml");
       destroy_dom_tree(document);
       return 0;
     }

   You may compile it with

   .. code-block:: bash 

     gcc -o test <above_source_file> -I<folder path where node.h and lxq_parser.h are kept>

   If you use a memory leak checker you'll notice that there are no leaks, so this actually destroys all nodes.

=============
Serialize DOM
=============

This section explains how to serialize a DOM tree into a string. Currently we support three different types of output format:

- XML - print the DOM tree as XML format.
- JSON - print the DOM tree as JavaScript Object Notation format.
- YAML - print the DOM tree as YAML Ain't Markup Language format.

To do this we've defined an enumerate as::

  typedef enum{
    XML=0,
    JSON=1,
    YAML=2
  } serialization_type;

The serializarion is done throw the function:

.. c:function:: char* node_to_string(dom_node* root, serialization_type t)

   :c:member:`root` The root of the tree to serialize onto a string. Can be any part of the DOM tree.

   :c:member:`t` The type of serialization to do. Must be one of XML, JSON, YAML.

   The following example shows how to serialize a document onto JSON format. If another one is desired you can change the type of serialization::

   #include <stdio.h>
   #include "include/node.h"
   #include "include/serialize.h"
   #include "include/lxq_parser.h"

   int main(int argc, char** argv){
     doc* document = parse_xml("some.xml");
     char* json;
     json = node_to_string(get_doc_root(document), JSON);
     printf("%s", json);
     free(json);
     return 0;
   }
