
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
	return (ptr = sbrk(size)) ? p : NULL;
}

int main() {
	Cell arr[2];

	printf("Before allocating: %x\n", sbrk(0));
	arr[0].size = 32;
	arr[0].ptr = meh_malloc1(32);
	printf("Allocating %d bits\n", 32);
	printf("After allocating: %x\n", sbrk(0));

	printf("Before alloc: %x\n", sbrk(0));
	arr[1].size = 16;
	arr[1].ptr = meh_malloc1(16);
	printf("Allocating %d bits\n", 16);
	printf("After allocating: %x\n", sbrk(0));
}
