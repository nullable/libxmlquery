include ../Makefile.inc

all: lex y.tab.h yacc ../include/lxq_parser.h lxq_parser.c
	$(CC) $(CFLAGS) -c lxq_parser.c
	$(CC) $(LEXFLAGS) lex.yy.c y.tab.c

lex y.tab.h yacc: parser.y parser.l
	yacc -d parser.y
	lex parser.l
	mv y.tab.h ../include/

clean:
	rm -rf *~ *.o *.out

