main: test.o malloc.o
	gcc -o malloc_test test.o malloc.o

malloc.o: malloc.c malloc.h
	gcc -O -c malloc.c

test.o: test.c malloc.h
	gcc -O -c test.c

clean:
	rm -f malloc_test test.o malloc.o