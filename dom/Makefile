include ../Makefile.inc

OBJS = node.o numbers.o bson.o serialize.o huffman.o
includes = ../include/node.h ../include/bson.h ../include/serializer.h ../include/bitbuffer.h ../includes/huffman.h

all: $(OBJS)

%.o: %.c $(includes)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *~ *.out $(OBJS)

