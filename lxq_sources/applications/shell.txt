.. highlight:: c

Shell
=====

The shell application is the most simplest application we've created to demonstrate the features of our querying engine. It has a simple interface and its purpose
is to load xml files, print them and query them.

Compiling
^^^^^^^^^

Compiling the shell is very easy. You just type make and it'll generate a binary called *shell*.

Running
^^^^^^^

After compiling the shell application, you just need to run the binary file *shell* without any arguments.

Interaction
^^^^^^^^^^^

After running the application, you'll be presented with a banner and a prompt. You can begin by typing *help* to see which commands it supports.

As an example::

   ############################################################################
   #                                                                          #
   #  ll   iii bbb  xx   xx mm   mm ll     qqqq   uu  uu eeeee rrrr  yy   yy  #
   #  ll    i  bb b  xx xx  mmm mmm ll    qq  qq  uu  uu ee    rr rr  yy yy   #
   #  ll    i  bbb    xxx   mmmmmmm ll   qq    qq uu  uu eeee  rrrr    yyy    #
   #  ll    i  bb b  xx xx  mm m mm ll    qq  qq  uu  uu ee    rr rr   yyy    #
   #  llll iii bbb  xx   xx mm   mm llll   qqqq q uuuuuu eeeee rr  rr  yyy    #
   #                                                                          #
   ############################################################################

   >>> help
   gdb:
   Call gdb for a given program
           Usage:gdb <binary_file>
   load:
   Load an xml file into a variable.
           Usage: load <file_to_load> <variable_name>

   help:
   Print this help message

   query:
   Query an existing xml document
           Usage:query <variable_name> "<query_expression>"

   quit:
   Exit from shell

   print_xml:
   Print a loaded document into xml format
           Usage: print_xml <variable_name>
   >>>

From here you can deduce what commands we implemented. The *gdb* command is there just for development convenience. A typical interaction would be::

   ############################################################################
   #                                                                          #
   #  ll   iii bbb  xx   xx mm   mm ll     qqqq   uu  uu eeeee rrrr  yy   yy  #
   #  ll    i  bb b  xx xx  mmm mmm ll    qq  qq  uu  uu ee    rr rr  yy yy   #
   #  ll    i  bbb    xxx   mmmmmmm ll   qq    qq uu  uu eeee  rrrr    yyy    #
   #  ll    i  bb b  xx xx  mm m mm ll    qq  qq  uu  uu ee    rr rr   yyy    #
   #  llll iii bbb  xx   xx mm   mm llll   qqqq q uuuuuu eeeee rr  rr  yyy    #
   #                                                                          #
   ############################################################################

   >>> load ../test.xml doc
   XML ../test.xml loaded and stored in doc
   >>> query doc "@title"
   Node:
   <title>
     RSS Example
   </title>
   
   Node:
   <title>
     Item Example
   </title>
   
   >>> quit
   exiting...

This example shows how to load and query a document.
