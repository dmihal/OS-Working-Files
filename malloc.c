
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ALLOC_SIZE 32

void *meh_malloc1(unsigned int size);
void *my_sbrk(unsigned int size);

typedef struct {
	void *ptr;
	int size, used;
} Cell;

void *meh_malloc1(unsigned int size) {  
	void *ptr;
	return (ptr = my_sbrk(ALLOC_SIZE)) ? ptr : NULL;
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

void *my_sbrk(unsigned int size) {
	static char *start = NULL, *end = NULL;
	char *ptr;

	// larger than buffer size
	if (size > ALLOC_SIZE)
		return sbrk(size);

	// buffer needs to be expanded
	if (end - start < size) {
		ptr = sbrk(ALLOC_SIZE);
		if (ptr == NULL)
			return NULL;
			
		// if buffers aren't adjacent, reset start
		if (ptr != end)
			start = ptr;
		end = ptr + ALLOC_SIZE;
	}

	// use part of buffer
	ptr = start;
	start += (size + 7) & ~7;

	return ptr;
}