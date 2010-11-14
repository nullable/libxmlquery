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

.. c:function:: char* set_name(dom_node* node, char* name)

.. c:function:: char* set_value(dom_node* node, char* value)

.. c:function:: dom_node* set_doc_root(doc* document, struct snode* root)

.. c:function:: dom_node* set_xml_declaration(doc* document, struct snode* vers)

.. c:function:: void set_parent(dom_node* node, dom_node* parent)

.. c:function:: char* get_namespace(dom_node* node)

.. c:function:: char* get_name(dom_node* node)

.. c:function:: char* get_value(dom_node* node)

.. c:function:: dom_node* get_xml_declaration(doc* document)

.. c:function:: dom_node* get_doc_root(doc* document)

.. c:function:: void prepend_child(dom_node* parent, dom_node* child)

.. c:function:: void append_child(dom_node* parent, dom_node* child)

.. c:function:: void append_children(dom_node* parent, struct generic_list_s* children)

.. c:function:: void add_attribute(dom_node* node, dom_node* attribute)

.. c:function:: doc* new_document(struct snode* xml_declaration)

.. c:function:: dom_node* new_element_node(char* name)

.. c:function:: dom_node* new_text_node(char* text)

.. c:function:: dom_node* new_attribute(char* name, char* value)

.. c:function:: dom_node* new_cdata(char* cdata_text)

.. c:function:: dom_node* get_child_at(dom_node* parent, int index)

.. c:function:: struct generic_list_s* get_children(dom_node* node)

.. c:function:: struct generic_list_s* get_descendants(dom_node* node)

.. c:function:: struct generic_list_s* get_text_nodes(doc* root)

.. c:function:: dom_node* get_attribute_by_name(dom_node* node, char* attr_name)

.. c:function:: struct generic_list_s* get_elements_by_name(doc* root, char* name)

.. c:function:: dom_node* delete_attribute(dom_node* node, char* name)

.. c:function:: void remove_node(doc* root, dom_node* node)

.. c:function:: void destroy_dom_node(dom_node* n)

.. c:function:: void destroy_dom_tree(doc* root)

.. c:function:: void print_node_list(struct generic_list_s* l)
