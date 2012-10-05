// Dylan Kirby djkirby
// David Mihal dimihal
// Project 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
	unsigned int size;
	void* prev;
	void* next;
} header;

const int headsize = sizeof(header);

void* malloc_addr;
int free_size;

header* malloc_head = NULL;

void *malloc(size_t size) {
	header *head, *last, *top_head, *new_head, *temp;
	void *ptr = NULL;
	head = malloc_head;
	int* sizeint;
	int newsize;
	last = NULL;

	while(head)
	{
		last = head;
		if (head->size >= (size + 8))
		{
			ptr = head;
			if (temp = head->prev){
				temp->next = head->next;
			}
			if (temp = head->next){
				temp->prev = head->next;
			}
		} else {
			head = head->next;
		}
	}
	if (!ptr)
	{
		int newsize = (size+8+headsize)*2;
		ptr = sbrk(newsize+headsize);
		top_head = ptr;
		top_head->prev = last;
		top_head->next = NULL;
		top_head->size = newsize;
		puts("4");
		if (last) {
			puts("test1");
			last->next = top_head;
			puts("test2");
		}
		puts("5");
		last = top_head;
		puts("6");
		ptr += headsize;
	}
	puts("7");
	sizeint = ptr;
	(*sizeint) = size;
	ptr +=8;

	new_head = ptr + size;
	new_head->size = last->size - (size+8+headsize);
	new_head->next = NULL;
	new_head->prev = last;
	last->next = new_head;
	
	return ptr;
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

void printMemory()
{
	header *c = malloc_head;
	printf("╔════════════╗\n");
	while(c)
	{
		printf("║ Free: %4d ║\n",c->size);
		c = c->next;
	}
	printf("╚════════════╝\n");
	
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
