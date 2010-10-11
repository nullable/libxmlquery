include ../Makefile.inc

all: parser.y parser.l dom_parser.h dom_parser.c
	yacc -d parser.y
	lex parser.l
	$(CC) $(CFLAGS) -c dom_parser.c
	$(CC) -ll -c lex.yy.c y.tab.c

clean:
	rm -rf *~ *.o lex.yy.c y.tab.[hc] *.out