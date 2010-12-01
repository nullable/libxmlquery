.. highlight:: c

========
Querying
========

Querying xml nodes to provide quick *random* access to inner nodes is
implemented through `CSS3 selectors <http://www.w3.org/TR/css3-selectors/>`_.

.. note::
    `CSS <http://en.wikipedia.org/wiki/Cascading_Style_Sheets>`_ stands for
    cascading style sheets, it is primarily used for styling (x)html documents
    as method to separate content from presentation.

The CSS3 selectors specification is a `W3C <http://www.w3.org/>`_ proposed
recomendation widely implemented by most modern browsers, it has also been used,
in its current and earlier forms (CSS and CSS2), by web developers for over 13
years.

The W3C recomendation does not tie CSS rules to the presentation of of (x)html
documents, in fact it does define it as a query language capable of selecting
xml/html nodes. With the rise in popularity of AJAX technologies several
javascript libraries became widely used, these libraries (most notabily
`JQuery <http://www.jquery.com>`_) first proposed the utilization of CSS rules
not only for presentation but also to help manipulation and transversion of the
DOM.

This library embraces CSS rules as a better/simpler alternative to manual
transversion as well as other W3C recomendations such as
`XPath <http://www.w3.org/TR/xpath/>`_ and
`XQuery <http://www.w3.org/TR/xquery/>`_.


Selectors
---------

The function `xmlquery_query` is defined to help filter and match nodes in the
:doc:`DOM tree <dom>`:

.. c:function:: generic_list* xmlquery_query(const char* pattern, dom_node* root)

   :c:member:`pattern` Query pattern to apply to XML document.

   :c:member:`root` A root of a DOM tree where to begin the query.

   This function applies a query to a given DOM tree and returns a list of elements.

Introduction
^^^^^^^^^^^^

Node selection is implemented as a **pattern string** to be matched against a
:doc:`DOM tree <dom>`. The pattern string matches some number of nodes that are
then returned as a flat list.

A pattern example:

.. code-block:: css

    foo

this simple pattern matches all nodes named `foo`. Upon matching the DOM tree
with this pattern, a reference to every node named `foo` would be returned.
Independent of their position in the tree. Returned references may be
manipulated, rearanged or deleted.


Pattern strings match dom nodes in three different realms, according to the node
name (as we've seen), according to the node attributes and according to
*pseudo-filters*, a special kind of selectors aware of the node context.

.. code-block:: css

    foo[attr='bar']:only-child

A more complex selector, this selector will match nodes with the name *foo*,
with an attribute named *attr* with the value *bar*, and the node must also be
the *only child* of its parent node.


Custom filters
^^^^^^^^^^^^^^

We've created some new features. We added the possibility of making *custom filters*.
*Custom filters* provide a way for the user to define their own filters, which can be
chained with other query operators.

To create a *custom filter* you just need to define a function with the one of the following signatures::

   int simple_filter(dom_node* node);
   int complex_filter(dom_node* node, list* args);

The first one creates a filter that doesn't take any arguments. If you want to design a filter that takes arguments, you must use the second signature.

After defining your filter you need to register them. You do this by calling:

.. c:function:: void register_simple_custom_filter(const char* name, int (*filter)(dom_node* node))

.. c:function:: void register_custom_filter(const char* name, int (*filter)(dom_node* node, list* args))

The first function registers a simple filter, while the second registers a complex one.

When you're done with this you can call your filters just like a CSS3 *filter*.

When your program finishes, we should call::

    void destroy_custom_filters()

Custom operators
^^^^^^^^^^^^^^^^

Another feature we've added is the possibility to create your own operators. 

Like custom filters, custom operators need to be registered in order to be used::

    void register_simple_custom_operator(const char* name, list* (*operator)(list* nodes));

The first argument is the name of the operator. This name will be used to call the operator. You can call your operator with an exclamation point. For example, if we register the operator *second* we can call it with "!second".

The second argument is a pointer to a function that applies the operator. This function must be defined and provided by the user.

We already provide two custom operators that we call extended operators. They have the following mean:

- first - Returns the first occurrence of an element. If you write "device!first" it will return the first device element.

- text-children - Returns the text below an element. If you write "device!text-children" it will return all the text nodes directly below elements with tag device.

However, in order to use them you need to call::

    register_extended_operators()

and when you're done you should call::

    destroy_custom_operators()
