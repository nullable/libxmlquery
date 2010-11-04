include ../Makefile.inc

PYTHON = ./python

all: helpers.o
	make -C $(PYTHON) all

helpers.o: helpers.c helpers.h
	$(CC) $(CFLAGS) -c helpers.c

clean:
	make -C $(PYTHON) clean
	rm helpers.o

