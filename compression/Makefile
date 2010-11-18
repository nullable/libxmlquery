include ../Makefile.inc


OBJS = huffman.o lzw.o
includes = ../include/bitbuffer.h ../includes/huffman.h ../include/byte_buffer.h ../include/btree.h ../include/stack.h


all: $(OBJS)

%.o: %.c $(includes)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *~ *.out $(OBJS)

