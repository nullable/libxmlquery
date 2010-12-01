.. highlight:: c

========
Tutorial
========

In this tutorial you'll learn how to parse, query and manipulate XML files. We're going to use the test.xml document, which is an RSS Feed and has the content:

.. code-block:: xml

 <?xml version="1.0" encoding="UTF-8" ?>
 <rss version="2.0">

   <channel>
    <title>RSS Example</title>
    <description>This is an example of an RSS feed</description>
    <link>http://www.domain.com/link.htm</link>
    <lastBuildDate>Mon, 28 Aug 2006 11:12:55 -0400 </lastBuildDate>
    <pubDate>Tue, 29 Aug 2006 09:00:00 -0400</pubDate>

    <item>
      <title>Item Example</title>
      <description>This is an example of an Item</description>
      <link>http://www.domain.com/link.htm</link>
      <guid isPermaLink="false"> 1102345</guid>
      <pubDate>Tue, 29 Aug 2006 09:00:00 -0400</pubDate>
    </item>

   </channel>
 </rss>

Every example in this tutorial must be compile as:

   .. code-block:: bash 
   
      gcc -I<path to our header files> ./libxmlquery.so <source_file>

Lets begin with parsing. The below code shows how to parse the above XML::

  #include "node.h"
  #include "lxq_parser.h"

  doc* global_document = NULL;

  void read_file(){
    global_document = parse_xml("test.xml");
  }

  void cleanup(){
    if(global_document)
      destroy_dom_tree(global_document);
  }      

  int main(int argc, char** argv){
    read_file();
    cleanup();
    return 0;
  }
 
Ok so this is a pretty simple file. We're creating a DOM tree of the above XML document and then cleaning it up. However, this is not a really helpfull example. Lets try to add
the capability of querying the document through the program arguments::

  #include <stdio.h>
  #include "node.h"
  #include "lxq_parser.h"
  #include "query_runner.h"

  doc* global_document = NULL;

  void read_file(){
    global_document = parse_xml("test.xml");
  }

  void cleanup(){
    if(global_document)
      destroy_dom_tree(global_document);
  }      

  int main(int argc, char** argv){
    //check if we have exactly 1 argument
    if(argc != 2){
      printf("Usage: %s \"query\"\n", argv[0]);
      return -1;
    }

    read_file();

    query(argv[1], get_doc_root(global_document));

    cleanup();
    return 0;
  }
    
Go ahead and run it with

   .. code-block:: bash 

      ./a.out "@title/"

It will search the document for all elements named "title".

However, we aren't able to see the output. Lets add that feature::

  #include <stdio.h>
  #include "node.h"
  #include "lxq_parser.h"
  #include "query_runner.h"
  #include "stack.h"
  #include "serialize.h"

  doc* global_document = NULL;

  void read_file(){
    global_document = parse_xml("test.xml");
  }

  void cleanup(){
    if(global_document)
      destroy_dom_tree(global_document);
  }      

  int main(int argc, char** argv){
    //check if we have exactly 1 argument
    if(argc != 2){
      printf("Usage: %s \"query\"\n", argv[0]);
      return -1;
    }

    read_file();

    list *result = query(argv[1], get_doc_root(global_document));
    
    generic_list_iterator* i = new_generic_list_iterator(result);
    while(generic_list_iterator_has_next(i)){
      dom_node* el = (dom_node*) generic_list_iterator_next(i);
      char* output = node_to_string(el, XML);
      printf("%s\n", output);
      free(output);
    }
    destroy_generic_list_iterator(i);

    cleanup();
    return 0;
  }

Try it with the same query as before and then try to retrieve the element "title" for the "item" with:

   .. code-block:: bash 

      ./a.out "@item>title"

Ok we're able to retrieve any thing from our RSS Feed. But now, we want to change the title of the element "item" to "no-title". Let's do it::

  #include <stdio.h>
  #include "node.h"
  #include "lxq_parser.h"
  #include "query_runner.h"
  #include "stack.h"
  #include "serialize.h"

  doc* global_document = NULL;

  void read_file(){
    global_document = parse_xml("test.xml");
  }

  void cleanup(){
    if(global_document)
      destroy_dom_tree(global_document);
  }      

  int main(int argc, char** argv){
    read_file();

    list *result = query("@item>title", get_doc_root(global_document));
    
    generic_list_iterator* i = new_generic_list_iterator(result);
    while(generic_list_iterator_has_next(i)){
      dom_node* el = (dom_node*) generic_list_iterator_next(i);
      //get the node which has the title
      dom_node* text_node = get_child_at(el, 0);
      char* old = set_value(text_node, "no-title");
      if(old)
        free(old);
    }
    destroy_generic_list_iterator(i);

    char* output = node_to_string(get_doc_root(global_document), XML);
    printf("%s\n", output);
    free(output);

    cleanup();
    return 0;
  }

Notice that we've removed the capabillity of using the arguments passed to the program. To run this example you just need to run the program without arguments.

This concludes the tutorial. We've shown how to parse, query and manipulate XML documents. The rest of the documentation explains how to add new features such as custom filters, custom operators and regular expressions.
