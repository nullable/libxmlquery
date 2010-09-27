DOM_SRC = ./dom
PARSER_SRC = ./parser

all:
	make -C $(DOM_SRC) all
	make -C $(PARSER_SRC) parser

clean:
	make -C $(DOM_SRC) clean
	make -C $(PARSER_SRC) clean