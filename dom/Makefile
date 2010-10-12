include ../Makefile.inc

OBJS = list.o tree.o node.o eregex.o

all: $(OBJS)

node.o: node.h node.c macros.h
	$(CC) $(CFLAGS) -c node.c

clean:
	rm -rf *~ *.out $(OBJS)