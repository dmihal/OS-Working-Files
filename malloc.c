// Dylan Kirby djkirby
// David Mihal dimihal
// Project 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//void *malloc(unsigned int size);

typedef struct {
	void *ptr;
	int size, used;
} Cell;

typedef struct {
	header* ptr;
	unsigned int size;
} header;

const int headsize = sizeof(header);

void* malloc_addr;
int free_size;

void *malloc(size_t size) {
	header* head;
	void *ptr;
	//onst int headsize = sizeof(header);
	if (malloc_addr && free_size >= (size+headsize))
	{
		printf("using existing memory\n");
		ptr = malloc_addr;
		malloc_addr = malloc_addr + size + headsize;
		free_size = free_size - size - headsize;
	} else {
		printf("calling sbrk\n");
		ptr = sbrk((size+headsize)*2);
		if(!ptr){
			return NULL;
		}

		malloc_addr = ptr + size + headsize;
		free_size = size + headsize;
	}
	head = ptr;
	head->ptr = NULL;
	head->size = size;
	
	return ptr +headsize;
}
void free(void* addr){
	return;
}

void *calloc(size_t count, size_t size) {
	void *ptr;
	int total_size = count * size;
	ptr = malloc(total_size);
	memset(ptr, '0', total_size);
	return ptr;
}
