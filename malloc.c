// Dylan Kirby djkirby
// David Mihal dimihal
// Project 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *malloc(unsigned int size);

typedef struct {
	void *ptr;
	int size, used;
} Cell;

void* malloc_addr;
int free_size;

void *malloc(size_t size) {
	int* sizeblock;
	void *ptr;
	if (malloc_addr && free_size >= (size+8))
	{
		printf("using existing memory\n");
		ptr = malloc_addr;
		malloc_addr = malloc_addr + size + 8;
		free_size = free_size - size - 8;
	} else {
		printf("calling sbrk\n");
		ptr = sbrk(size*2+16);
		if(!ptr){
			return NULL;
		}

		malloc_addr = ptr + size + 8;
		free_size = size+8;
	}
	sizeblock = ptr;
	(*sizeblock) = size;
	
	return ptr +8;
}
void free(void* addr){
	return;
}

/*void *calloc(ßsigned int count, unsigned int size) {
	void *ptr;
	int total_size = count * size;
	ptr = malloc(total_size);
	return ptr;
}
*/
