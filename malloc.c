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
	void* ptr;
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

void *realloc(void *ptr, size_t size) {
	header* head = ptr;
	int cur_size = head->size;
	int ptr_size = sizeof(ptr);
	int free = ptr_size = cur_size;
/*
	 If there is not enough room to
     enlarge the memory allocation pointed to by ptr, realloc() creates a new
     allocation, copies as much of the old data pointed to by ptr as will fit
     to the new allocation, frees the old allocation, and returns a pointer to
     the allocated memory.  
     
     If ptr is NULL, realloc() is identical to a call
     to malloc() for size bytes.  
     
     If size is zero and ptr is not NULL, a new,
     minimum sized object is allocated and the original object is freed.  When
     extending a region allocated with calloc(3), realloc(3) does not guaran-
     tee that the additional memory is also zero-filled.
*/
	if (ptr == NULL) {
 		ptr = malloc(size);
 		return ptr;
 	} else if (size == 0) { 
		//free(ptr);
		return NULL;
	} else if (size <= free) {
		head->size = size;
		return ptr;
	} else {
		void *newptr = malloc(size);
		memcpy(newptr, ptr, cur_size);
		return newptr;
	}
	
}
