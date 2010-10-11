include ../Makefile.inc

all: query_parser.o
	

query_parser.o: query_parser.c query_parser.h
	$(CC) $(CFLAGS) -c query_parser.c
clean:
	rm -rf *~ *.out