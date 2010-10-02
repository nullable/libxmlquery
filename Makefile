DOM_SRC = ./dom
PARSER_SRC = ./parser
STRUCTS = ./data_structures
OUT = dom_parser

all:
	make -C $(DOM_SRC) all
	make -C $(PARSER_SRC) all
	make -C $(STRUCTS) all
	$(CC) $(CFLAGS) -o $(OUT) $(PARSER_SRC)/*.o $(DOM_SRC)/*.o $(STRUCTS)/*.o main.c

clean:
	make -C $(DOM_SRC) clean
	make -C $(PARSER_SRC) clean
	make -C $(STRUCTS) clean
	rm -rf *~ *.o $(OUT)