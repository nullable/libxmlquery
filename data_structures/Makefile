include ../Makefile.inc

OBJS = rbtree.o stack.o btree.o bitbuffer.o

all: $(OBJS)

rbtree.o: ../include/rbtree.h rbtree.c
	$(CC) $(CFLAGS) -c rbtree.c
stack.o: ../include/stack.h stack.c
	$(CC) $(CFLAGS) -c stack.c
btree.o: ../include/btree.h btree.c
	$(CC) $(CFLAGS) -c btree.c
bitbuffer.o: ../include/bitbuffer.h bitbuffer.c
	$(CC) $(CFLAGS) -c bitbuffer.c


clean:
	rm -rf $(OBJS) *~ *.out

