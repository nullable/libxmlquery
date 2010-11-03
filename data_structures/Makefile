include ../Makefile.inc

OBJS = rbtree.o stack.o

all: $(OBJS)

rbtree.o: ../include/rbtree.h rbtree.c
	$(CC) $(CFLAGS) -c rbtree.c
stack.o: ../include/stack.h stack.c
	$(CC) $(CFLAGS) -c stack.c

clean:
	rm -rf $(OBJS) *~ *.out