include ../Makefile.inc

all: node.o

node.o: node.h node.c macros.h
	$(CC) $(CFLAGS) -c node.c

clean:
	rm -rf *~ *.out $(OBJS)