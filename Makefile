DOM_SRC = ./dom
PARSER_SRC = ./parser
OUT = dom_parser

all:
	make -C $(DOM_SRC) all
	make -C $(PARSER_SRC) all
	$(CC) $(CFLAGS) -o $(OUT) $(PARSER_SRC)/*.o $(DOM_SRC)/*.o main.c

clean:
	make -C $(DOM_SRC) clean
	make -C $(PARSER_SRC) clean
	rm -rf *~ *.o $(OUT)