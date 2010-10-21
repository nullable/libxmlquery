include ../Makefile.inc

OBJS = query_parser.o query_runner.o
includes = $(wildcard *.h)

all: $(OBJS)

%.o: %.c $(includes)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *~ *.out $(OBJS)

