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
	//free(teststr);
	//free(arr);
	
	int *p;
  	if (p = calloc(4,8)) {
  		int i = 0;
  		for(i = 0; i < 4; i++)
      		printf("%08x\n", p[i]);
  	} else {
    	printf("NULL\n");
    }
    //free(p);
    
    int *p1 = realloc(NULL, 10);
    strcpy(p1, "abcdefghi");
    if (p1) {
    	printf("%s\n", p1);
	    //free(p1);
	}

    int *p2 = malloc(10);
    strcpy(p2, "jklmnopqr");
    printf("%s\n", p2);
    p2 = realloc(p2, 0);
    if (p2) {
    	strcpy(p2, "123456789");
    	printf("%s\n", p2);
		free(p2);
	}
    
    int *p3 = malloc(10);
    strcpy(p3, "stuvwxyza");
    printf("%s\n", p3);
    p3 = realloc(p3, 15);
    if (p3) {
    	strcpy(p3, "bcdefghijklmno");
	    printf("%s\n", p3);
	    //free(p3);
	}
	
	//printMemory();
	return 0;
}