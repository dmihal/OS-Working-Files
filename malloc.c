// Dylan Kirby djkirby
// David Mihal dimihal
// Project 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void printMemory();

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
			break;
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
		if (last) {
			last->next = top_head;
		} else {
			malloc_head = top_head;
		}
		last = top_head;
		ptr += headsize;
	}
	sizeint = ptr;
	(*sizeint) = size;
	ptr +=8;

	new_head = ptr + size;
	new_head->size = last->size - (size+8+headsize);
	new_head->next = NULL;
	new_head->prev = last;
	last->next = new_head;
	last->size = 0;
	
	return ptr;
}
void free(void* addr){
	return;
}

void *calloc(size_t count, size_t size) {
	void *ptr;
	int total_size = count * size;
	if (ptr = malloc(total_size))
		memset(ptr, 0x00, total_size);
	return ptr;
}

void printMemory()
{
	header *c = malloc_head;
	puts("╔════════════╗");
	while(c)
	{
		printf("║ Free: %4d ║\n",c->size);
		c = c->next;
	}
	puts("╚════════════╝");
	
}

void *realloc(void *ptr, size_t size) {	
	int ptr_size = sizeof(ptr);
	int free = ((header*)ptr)->size;

	if (ptr == NULL) {
 		ptr = malloc(size);
 		return ptr;
 	} else if (size == 0) { 
		void* newptr = malloc(sizeof(ptr));
     	//free(ptr);
     	return newptr;
	} else if (size <= free) {
		((header*)ptr)->size -= size;
		return ptr;
	} else {
		void *newptr = malloc(size);
		memcpy(newptr, ptr, sizeof(ptr));
		return newptr;
	}
}
