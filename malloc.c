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
	int emptysize;
	last = NULL;

	while(head)
	{
		last = head;
		if (head->size >= (size + 8))
		{
			ptr = head;
			emptysize = head->size;
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
		emptysize = (size+8+headsize)*2;
		ptr = sbrk(emptysize+headsize);
	}
	sizeint = ptr;
	(*sizeint) = size;
	ptr +=8;

	new_head = ptr + size;
	new_head->size = emptysize - (size+8+headsize);
	new_head->next = NULL;
	if (last)
	{
		new_head->prev = last;
		last->next = new_head;
	} else {
		malloc_head = new_head;
	}
	
	return ptr;
}
void free(void* addr){
	int size;
	int* sizeptr;
	void* ptr;
	header *new, *next, *prev;

	/*addr -= 8;
	sizeptr = addr;
	size = *sizeptr;//*((int*)addr);*/
	new = addr;
	//new->size = size - headsize;

	ptr = malloc_head;
	while(ptr && ptr < addr){
		next = ptr;
		ptr = next->next;
	}
	
	prev = next->prev;

	next->prev = new;
	new->next = next;

	if (prev){
		prev->next = new;
	}
	new->prev = prev;

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
    if (size == 0) {
        //free(ptr);
        return NULL;
    } else if (ptr == NULL) {
        return malloc(size);
    } else {
        void *new;
        if (new = malloc(size)) {
            memcpy(new, ptr, headsize);
            //free(ptr);
        }
        return new;
    }
}
