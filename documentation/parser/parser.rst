.. highlight:: c

=============
Parser Module
=============

The parser module is currently done with the flex lib and yacc. This forced us to merge the xml and query parser onto a single lexer file.

Be aware that the parser is not thread safe and because it uses global variables, xmls and queries have to be parsed one at a time.

Our implementation is divided into 3 files - parser.l, parser.y and lxq_parser.c. The parser.l file has all the lexer code. The parser.y defines the yacc grammar and a couple of functions explained in the next section. The lxq_parser.c defines two functions which are front ends to the parser and through where all parsing should be done.

If for some reason your program exits without priting any message, you should recompile libxmlquery with ``-DDEBUG``, which may report extra error messages.

Function description
^^^^^^^^^^^^^^^^^^^^

.. c:function:: void parse_file(char* filename)

   :c:member:`filename` The name of the file to be parsed

   This function maps the file in memory and parses it. The parsed structure is accessible through ``lxq_document``, which is a global variable declared in parser.y. If you need to parse an xml, we strongly recommend that you use :c:func:`parse_dom` .

.. c:function:: void parse_string(const char* str)

   :c:member:`str` The string to be parsed.

   This function parses the input string and stores the result in ``lxq_selected_elements``. If you need to parse a query, we stringly recommend that you use :c:func:`parse_query` .

.. c:function:: doc* parse_xml(char* filename)

   :c:member:`filename` The name of the xml file to be parsed.

   This function parses an xml file, copying the result from :c:func:`parse_file` to another location so you can parse as many xml files as possible. If an error occurs, this function returns NULL.

   The following example shows how to use this function to parse an xml.

   Example::

     #include <stdio.h>
     #include "serialize.h"
     #include "lxq_parser.h"

     int main(){
       doc* document = parse_xml("xml_file.xml");
       char* parsed = node_to_string(get_doc_root(document), XML);
       
       printf("%s", parsed);
       free(parsed);

       if(document != NULL)
         destroy_dom_tree(document);

       return 0;
     }

   You may compile it with

   .. code-block:: bash 

     gcc -o test <above_source_file> -I<folder path where our header files are kept>

.. c:function:: doc* parse_xml_from_string(char* xmlstring)

   :c:member:`xmlstring` The the xml string to be parsed.

   This function parses an xml string, copying the result from :c:func:`parse_string` to another location so you can parse as many xml strings as possible. If an error occurs, this function returns NULL.

   The following example shows how to use this function to parse an xml string.

   Example::

     #include <stdio.h>
     #include "serialize.h"
     #include "lxq_parser.h"

     int main(){
       doc* document = parse_xml_from_string("<root> <someelement with=\"someattribute\" /> </root>");
       char* parsed = node_to_string(get_doc_root(document), XML);
       
       printf("%s", parsed);
       free(parsed);

       if(document != NULL)
         destroy_dom_tree(document);

       return 0;
     }

   You may compile it with

   .. code-block:: bash 

     gcc -o test <above_source_file> -I<folder path where our header files are kept>
