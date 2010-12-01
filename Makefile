include Makefile.inc

DOM_SRC = ./dom
PARSER_SRC = ./parser
STRUCTS = ./data_structures
SELECTORS = ./selectors

all: so

object_code:
	make -C $(DOM_SRC) all
	make -C $(PARSER_SRC) all
	make -C $(STRUCTS) all
	make -C $(SELECTORS) all

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