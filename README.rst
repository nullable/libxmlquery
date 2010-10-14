===
DOM
===

After the parsing of the xml is done, a DOM structure is created and can be accessed
through a global variable denominated "document". Bear in mind that this variable is
dependent of the current parser. In other words, if the parser module changes, then
the variable name may also change.

The DOM structure is a tree, where each node is an xml element. Since the input xml
has already been validated we know it has only one root, so the document variable
will point to a single node in the dom tree, the root node (This can be accessed
through document->root).

The dom structure provides flexible and extendable xml document handling. It has the
capability to add, remove and modify elements in the xml. The next section will
explain in deeper detail the DOM module functions.

DOM functions
-------------

To create a dom structure, the function new_document should be called. If the input xml has the form::

    <?xml version="1.0"?>
    <root>
      ....
    </root>

And you wish to store the <?xml version="1.0" ?>, you can do this by creating a
dom_node with the name "xml" and attribute "version". This dom_node must be passed
to the function new_document. This is done to keep the dom structure simpler and
with only one root. As we said before, this implementation makes the assumption that
the xml has already been validated.

Although we provide in the header file the structures dom_node and doc, thus exposing
their implementation, we highly recommend that you use the proper access functions to
obtain each field. If we change the structures, we will not change the functions'
signature, only their implementation if needed. By using the access methods, you
won't need to change any code done. For the same reason, we recommend that you use the
proper setter functions to store the values in each node.

