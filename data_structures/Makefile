include ../Makefile.inc

OBJS = rbtree.o stack.o byte_buffer.o

all: $(OBJS)

rbtree.o: ../include/rbtree.h rbtree.c
	$(CC) $(CFLAGS) -c rbtree.c
stack.o: ../include/stack.h stack.c
	$(CC) $(CFLAGS) -c stack.c
byte_buffer.o: ../include/byte_buffer.h byte_buffer.c
	$(CC) $(CFLAGS) -c byte_buffer.c

clean:
	rm -rf $(OBJS) *~ *.out