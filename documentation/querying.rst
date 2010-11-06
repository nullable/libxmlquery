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


.. c:function:: dom_node* xmlquery_query_one(const char* pattern, dom_node* root)

    This is a helper function designed to facilitate selection of single nodes.
    Selects the first node returned by a given query, it is equivalent to the
    following code::

        dom_node* single = NULL;
        generic_list* l = xmlquery_query(patterm, root)
        if((l != NULL && l->count > 0)
            single = (dom_node*) get_element_at(l, 0);


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

