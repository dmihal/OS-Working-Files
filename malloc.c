// Dylan Kirby djkirby
// David Mihal dimihal
// Project 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *meh_malloc1(unsigned int size);

typedef struct {
	void *ptr;
	int size, used;
} Cell;

void* malloc_addr;
int free_size;

void *meh_malloc1(unsigned int size) {
	int* sizeblock;
	void *ptr;
	if (malloc_addr && free_size >= (size+8))
	{
		ptr = malloc_addr;
		malloc_addr = malloc_addr + size + 8;
		free_size = free_size - size - 8;
	} else {
		ptr = sbrk(size*2+16);
		if(!ptr){
			return NULL;
		}

		malloc_addr = ptr + size + 8;
		free_size = size-8;
	}
	sizeblock = ptr;
	(*sizeblock) = size;
	
	return ptr +8;
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
