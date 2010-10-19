include ../Makefile.inc

OBJS = node.o numbers.o bson.o serialize.o
includes = $(wildcard include/*.h)

all: $(OBJS)

%.o: %.c $(includes)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *~ *.out $(OBJS)

