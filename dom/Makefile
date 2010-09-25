include ../Makefile.inc

OBJS = list.o tree.o node.o eregex.o

all: $(OBJS)

tree.o: tree.h tree.c macros.h
	$(CC) $(CFLAGS) -c tree.c

list.o: list.h list.c macros.h
	$(CC) $(CFLAGS) -c list.c

node.o: node.h node.c macros.h
	$(CC) $(CFLAGS) -c node.c

eregex.o: eregex.h eregex.c
	$(CC) $(CFLAGS) -c eregex.c

clean:
	rm -rf *~ *.out $(OBJS)