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

void *calloc(size_t count, size_t size) {
	void *ptr;
	int total_size = count * size;
	ptr = malloc(total_size);
	memset(ptr, '0', total_size);
	return ptr;
}

void *realloc(void *ptr, size_t size) {
	header* head = ptr;
	int cur_size = (header*)head->size;
	int ptr_size = sizeof(ptr);
	int free = ptr_size = cur_size;
/*
If sufficient space exists to expand the memory block pointed to by ptr, the additional 
memory is allocated and the function returns ptr.

If memory is insufficient for the reallocation (either expanding the old block or 
allocating a new one), the function returns NULL, and the original block is unchanged.
*/
	if (ptr == NULL) {
 		ptr = malloc(size);
 		return ptr;
 	} else if (size == 0) { 
		free(ptr);
		return NULL;
	} else if (size <= free) {
		head->size = size;
		return ptr;
	} else {
		void *newptr = malloc(size);
		memcpy(newptr, *ptr);
		return newptr;
	}
	
}

