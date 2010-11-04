
SO = pyxmlquery.so

all: _libxmlquery.so

libxmlquery.py libxmlquery_wrap.c: libxmlquery.i
	swig -python libxmlquery.i

libxmlquery_wrap.o: libxmlquery_wrap.c
	gcc -c -fpic libxmlquery_wrap.c -I/usr/include/python2.6

../../libxmlquery.so:
	make -C ../.. so

_libxmlquery.so: libxmlquery_wrap.o ../../libxmlquery.so ../helpers.o
	gcc -shared ../../libxmlquery.so libxmlquery_wrap.o ../helpers.o -o _libxmlquery.so

../helpers.o: ../helpers.c ../helpers.h

clean:
	rm -rf libxmlquery.py libxmlquery_wrap.c libxmlquery_wrap.o _libxmlquery.so

