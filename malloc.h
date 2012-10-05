#ifndef ADD_H_GUARD
#define ADD_H_GUARD
void *calloc(size_t count, size_t size);
void *realloc(void *ptr, size_t size);
void *malloc(size_t size);
void free(void* addr);
void printMemory();
#endif
