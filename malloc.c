// Dylan Kirby djkirby
// David Mihal dimihal
// Project 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ALLOC_SIZE 40

void *meh_malloc1(unsigned int size);

void *meh_malloc1(unsigned int size) {  
	void *start, *end, *ptr;

	if (size > ALLOC_SIZE)
		return sbrk(size);

	if (end - start < size) {
		if ((ptr = sbrk(ALLOC_SIZE)) == NULL)
			return NULL;
		if (ptr != end)
			start = ptr;
		end = ptr + ALLOC_SIZE;
	}

	ptr = start;
	start += ALLOC_SIZE;

	return (ptr ? ptr : NULL);
}

int main() {
	int arr[2];

	printf("%x\n", sbrk(0));
	arr[0] = meh_malloc1(ALLOC_SIZE);
	printf("%x\n\n", sbrk(0));

	printf("%x\n", sbrk(0));
	arr[1] = meh_malloc1(48);
	printf("%x\n\n", sbrk(0));
}
