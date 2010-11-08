include ../Makefile.inc

OBJS = node.o numbers.o bdom.o serialize.o byte_buffer.o
includes = ../include/node.h ../include/bdom.h ../include/serializer.h ../include/byte_buffer.h

all: $(OBJS)

%.o: %.c $(includes)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *~ *.out $(OBJS)

