
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ALLOC_SIZE 32

void *meh_malloc1(unsigned int size);

typedef struct {
	void *ptr;
	int size;
} Cell;

void *meh_malloc1(unsigned int size) {  
	void *start, *end, *ptr;

	// larger than buffer size
	if (size > ALLOC_SIZE)
		return sbrk(size);

	// buffer needs to be expanded
	if (end - start < size) {
		if ((ptr = sbrk(ALLOC_SIZE)) == NULL)
			return NULL;
		if (ptr != end)
			start = ptr;
		end = ptr + ALLOC_SIZE;
	}

	// use part of buffer
	ptr = start;
	start += ALLOC_SIZE;

	return (ptr ? ptr : NULL);
}

int main() {
	Cell arr[2];

	printf("Before allocating: %x\n", sbrk(0));
	arr[0].size = ALLOC_SIZE;
	arr[0].ptr = meh_malloc1(ALLOC_SIZE);
	printf("Allocating %d bits\n", ALLOC_SIZE);
	printf("After allocating: %x\n\n", sbrk(0));

	printf("Before alloc: %x\n", sbrk(0));
	arr[1].size = ALLOC_SIZE;
	arr[1].ptr = meh_malloc1(ALLOC_SIZE);
	printf("Allocating %d bits\n", ALLOC_SIZE);
	printf("After allocating: %x\n\n", sbrk(0));
}
