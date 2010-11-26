include ../Makefile.inc

all: lex.yy.c ../include/y.tab.h y.tab.c ../include/lxq_parser.h lxq_parser.c
	$(CC) $(CFLAGS) -c lxq_parser.c
	$(CC) $(LEXFLAGS) lex.yy.c y.tab.c

lex.yy.c ../include/y.tab.h y.tab.c: parser.y parser.l
	yacc -d parser.y
	lex parser.l
	mv y.tab.h ../include/

clean:
	rm -rf *~ *.o *.out