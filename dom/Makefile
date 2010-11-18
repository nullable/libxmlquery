include ../Makefile.inc


OBJS = node.o numbers.o bdom.o serialize.o huffman.o
includes = ../include/node.h ../include/bdom.h ../include/serializer.h ../include/bitbuffer.h ../includes/huffman.h ../include/byte_buffer.h


all: $(OBJS)

%.o: %.c $(includes)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *~ *.out $(OBJS)

