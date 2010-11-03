include ../Makefile.inc

EXEC = shell

all: command.o
	$(CC) -o $(EXEC) $(CFLAGS) ../libxmlquery.so command.o symbol_table.o shell.c

command.o: symbol_table.o command.h command.c
	$(CC) $(CFLAGS) -c command.c

symbol_table.o: symbol_table.h symbol_table.c
	$(CC) $(CFLAGS) -c symbol_table.c

clean:
	rm -rf *.o $(EXEC) *~ 