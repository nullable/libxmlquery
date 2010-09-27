include ../Makefile.inc

OUT = parser

parser: parser.y parser.l
	yacc -d parser.y
	lex parser.l
	gcc -o $(OUT) $(CFLAGS) lex.yy.c y.tab.c

clean:
	rm -rf *~ *.o lex.yy.c y.tab.[hc] *.out $(OUT)