include ../Makefile.inc

all: parser.y parser.l ../include/dom_parser.h dom_parser.c
	yacc -d parser.y
	lex parser.l
	mv y.tab.h ../include/
	$(CC) $(CFLAGS) -c dom_parser.c
	$(CC) $(LEXFLAGS) lex.yy.c y.tab.c

clean:
	rm -rf *~ *.o lex.yy.c y.tab.c ../include/y.tab.h *.out