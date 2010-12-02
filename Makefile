include Makefile.inc

DOM_SRC = ./dom
PARSER_SRC = ./parser
STRUCTS = ./data_structures
SELECTORS = ./selectors
OUT = xmlquery

all: exec so

object_code:
	make -C $(DOM_SRC) all
	make -C $(PARSER_SRC) all
	make -C $(STRUCTS) all
	make -C $(SELECTORS) all

tests: so
	make -C tests

exec: object_code
	$(CC) $(CFLAGS) -o $(OUT) $(PARSER_SRC)/*.o $(DOM_SRC)/*.o $(STRUCTS)/*.o $(SELECTORS)/*.o main.c

clean:
	make -C $(DOM_SRC) clean
	make -C $(PARSER_SRC) clean
	make -C $(STRUCTS) clean
	make -C $(SELECTORS) clean
	rm -rf *~ *.o $(OUT) libxmlquery.so

so: object_code
	$(CC) $(CFLAGS) $(PARSER_SRC)/*.o $(DOM_SRC)/*.o $(STRUCTS)/*.o $(SELECTORS)/*.o -shared -o libxmlquery.so

install:
	./install.sh

uninstall:
	./install.sh -u

