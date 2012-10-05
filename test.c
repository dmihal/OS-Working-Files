#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "malloc.h"

int main(int argc, char const *argv[])
{
	int* arr[2];

	printf("%x\n", sbrk(0));
	arr[0] = malloc(48);
	printf("%x\n\n", sbrk(0));

	printf("%x\n", sbrk(0));
	arr[1] = malloc(30);
	printf("%x\n\n", sbrk(0));

	char* teststr = malloc(10);
	strcpy(teststr,"malloc!");

	printf("[%s]\n\n", teststr);
	
	int *str = calloc(2, 8);
	//strcpy(str, "abcdefghijklmno");
	printf("[%d]\n", (*str));
	printMemory();
	return 0;
}