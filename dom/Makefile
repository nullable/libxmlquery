include ../Makefile.inc


OBJS = node.o serialize.o
includes = ../include/node.h ../include/serializer.h ../include/bitbuffer.h ../include/byte_buffer.h


all: $(OBJS)

%.o: %.c $(includes)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *~ *.out $(OBJS)

