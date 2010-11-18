include ../Makefile.inc

OBJS = query_parser.o query_runner.o
includes = ../include/query_parser.h ../include/query_runner.h ../include/eregex.h

all: $(OBJS)

%.o: %.c $(includes)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *~ *.out $(OBJS)

