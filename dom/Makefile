include ../Makefile.inc

OBJS = tree.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS)

tree.o: tree.h tree.c
	$(CC) $(CFLAGS) -c tree.c

clean:
	rm -rf *~ *.out $(OBJS)