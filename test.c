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

	char* teststr = malloc(40);
	strcpy(teststr,"malloc!");

	printf("[%s]\n\n", teststr);
	
	int *p;
  	if (p = calloc(4,8)) {
  		int i = 0;
  		for(i = 0; i < 4; i++)
      		printf("%08x\n", p[i]);
  	} else {
    	printf("NULL\n");
    }
    
    char *s = malloc(8);
    strcpy(s, "abcdefg");
    printf("%s\n",s);
    s = realloc(s, 16);
    printf("%s\n",s);
    
    int *numbers, *morenumbers;
    morenumbers = (int*)realloc(numbers, 2*sizeof(int));
    
	
	printMemory();
	return 0;
}