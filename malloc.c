
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ALLOC_SIZE 32

void *meh_malloc1(unsigned int size);

typedef struct {
	void *ptr;
	int size, used;
} Cell;

void *meh_malloc1(unsigned int size) {  
	void *ptr;
	return (ptr = sbrk(size)) ? ptr : NULL;
}

int main() {
	Cell arr[2];

	printf("Before allocating: %x\n", sbrk(0));
	arr[0].size = ALLOC_SIZE;
	arr[0].ptr = meh_malloc1(ALLOC_SIZE);
	printf("Allocating %d bits\n", ALLOC_SIZE);
	printf("After allocating: %x\n", sbrk(0));

	printf("Before alloc: %x\n", sbrk(0));
	arr[1].size = ALLOC_SIZE;
	arr[1].ptr = meh_malloc1(ALLOC_SIZE);
	printf("Allocating %d bits\n", ALLOC_SIZE);
	printf("After allocating: %x\n", sbrk(0));
}
