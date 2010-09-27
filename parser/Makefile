include ../Makefile.inc

parser: parser.y parser.l parser.h parser.c
	yacc -d parser.y
	lex parser.l
	$(CC) $(CFLAGS) -c parser.c
	$(CC) $(CFLAGS) -ll -c lex.yy.c y.tab.c

clean:
	rm -rf *~ *.o lex.yy.c y.tab.[hc] *.out $(OUT)