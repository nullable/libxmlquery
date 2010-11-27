.. highlight:: c

========
Tutorial
========

The following code shows how to use the library to rename a node in a xml file::

    #include <stdio.h>
    #include <xmlquery/parse.h>
    #include <xmlquery/dom.h>
    #include <xmlquery/serialize.h>

    dom_node* node = xmlquery_parsefile("test.xml");
    node_list* q_result = xmlquery_query("@hi[foo=bar]");
    dom_node* n_result = xmlquery_pop_node(q_result);
    xmlquery_set_node_name(n_result, "bye");

    printf("%s\n", xmlquery_dom_node_to_string(node, XML);

if ``test.xml`` contains:

.. code-block:: xml

    <root>
      <hi />
      <hi foo="bar" />
    </root>

the resulting output would change the node:

.. code-block:: xml

    <hi foo="bar" />

to:

.. code-block:: xml

    <bye foo="bar" />


this code will read an xml file in ``test.xml`` into our abstract representation
of the DOM (\ **D**\ ocument **O**\ bject **M**\ odel) tree::

    dom_node* node = xmlquery_parsefile("test.xml");

