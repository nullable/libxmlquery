include ../Makefile.inc

all: exec tests

exec:
	$(CC) -o dom_parser $(CFLAGS) ../libxmlquery.so -I../include test.c

tests: exec
	./tests

