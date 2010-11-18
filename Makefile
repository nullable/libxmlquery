include Makefile.inc

DOM_SRC = ./dom
PARSER_SRC = ./parser
STRUCTS = ./data_structures
SELECTORS = ./selectors
COMPRESSION = ./compression
OUT = dom_parser

all: exec so

object_code:
	make -C $(DOM_SRC) all
	make -C $(PARSER_SRC) all
	make -C $(STRUCTS) all
	make -C $(SELECTORS) all
	make -C $(COMPRESSION) all

exec: object_code
	$(CC) $(CFLAGS) -o $(OUT) $(PARSER_SRC)/*.o $(DOM_SRC)/*.o $(STRUCTS)/*.o $(SELECTORS)/*.o $(COMPRESSION)/*.o main.c

clean:
	make -C $(DOM_SRC) clean
	make -C $(PARSER_SRC) clean
	make -C $(STRUCTS) clean
	make -C $(SELECTORS) clean
	make -C $(COMPRESSION) clean
	rm -rf *~ *.o $(OUT) libxmlquery.so

so: object_code
	$(CC) $(CFLAGS) $(PARSER_SRC)/*.o $(DOM_SRC)/*.o $(STRUCTS)/*.o $(SELECTORS)/*.o $(COMPRESSION)/*.o -shared -o libxmlquery.so

