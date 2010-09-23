DOM_SRC = ./dom

all:
	make -C $(DOM_SRC) all

clean:
	make -C $(DOM_SRC) clean