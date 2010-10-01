include ../Makefile.inc

all: parser.y parser.l query_parser.h query_parser.c
	yacc -d parser.y
	lex parser.l
	$(CC) $(CFLAGS) -c query_parser.c
	$(CC) -ll -c lex.yy.c y.tab.c

clean:
	rm -rf *~ *.o lex.yy.c y.tab.[hc] *.out $(OUT)