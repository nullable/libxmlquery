.. highlight:: c

==============
Special Macros
==============

The file macros.h contains macros used all over our project. These macros were made to ease our pain in coding some things. This page describes and shows how to use them

alloc
-----

This section describes the macro 

.. c:macro:: alloc

Ever get tired of calling malloc and check if the return address is a valid address, and if not print an error message and exit? The :c:macro:`alloc` macro can do all of this for you. It depends on the function :c:func:`__alloc`:

.. c:function:: static inline void* __alloc(void* x)

It is defined as::

   #define alloc(type, how_many)                           \
      (type *) __alloc(malloc(how_many * sizeof(type)));    

   static inline void* __alloc(void* x){
      if(x)
        return x;
      log(F,"malloc failed.");                              
      exit(1);
      return 0;
   }

The key to this macro resides in the :c:func:`__alloc` function. It receives the address returned by :c:func:`malloc` and checks if it is valid. If not, then a :c:macro:`log` message is printed and :c:func:`exit` function is called. The following example shows how to use this function to allocate an array of 100 characters::

   #include <stdio.h>
   #include "macros.h"

   int main(){
     char* buffer = alloc(char, 100);
     return 0;
   }

You may compile it with

   .. code-block:: bash 

     gcc -o test <above_source_file> -I<folder path where macros.h is kept>

Notice that you won't even have to bother with casting it to the right type.

log
---

This section describes the macro

.. c:macro:: log

Ever filled your code with :c:func:`printf` functions to debug it? And when you're done with it needed to erase them? The :c:macro:`log` adds proper debug messages and doesn't need to be removed when the code is functional. Let's look at its definition::

   #ifdef DEBUG
   #include <stdio.h>
   #define log(level, format, ...)                                 \
     do{                                                           \
       fprintf(stderr, "%s  %s:%d: ", level, __FILE__, __LINE__);  \
       fprintf(stderr, format, ## __VA_ARGS__);                    \
       fprintf(stderr, "\n");                                      \
     }while(0)
   #else
   #define log(level, str, ...)
   #endif

   #define I "INFO"
   #define W "WARNNING"
   #define E "ERROR"
   #define F "FATAL ERROR"

First note the macros for I, W, E, F at the bottom. These are merely alias for the level of severity, you may use any other. The :c:macro:`log` macro prints messages to the ``stderr`` and is able to format strings according to :c:func:`printf` format options. Consider this example::

   #include "macros.h"

   int main(){
    log(I, "Printing info");
    return 0;
   }

If we compile and run this example with the option :command:`-DDEBUG` we'll get the following output

   .. code-block:: bash

     Printing info

If such option is not passed to the compiler, them no output will be shown. This happens because the macro is not created. What this means is that you can actually fill the code with logging messages and debug it, and when you're done just compile it without :command:`-DDEBUG` . The result is a binary file that doesn't contain code related to debug messages, thus is smaller than the debug version. However, if you need to debug it again, you can always recompile it with the debug flag.

Notice that file name and line number are printed as well, which helps you pin-point the location of the print and check what may have been the cause of the problem.

