// Traffic using semaphores
// Dylan Kirby djkirby
// David Mihal dimihal

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>      
#include <stdlib.h>     
#include <string.h>     

void handler (void *ptr);

sem_t mutex;
int counter;

typedef struct {
	int approach;
	int turn;
} Car;

enum INTERSECTION_POSITION {
	NW,
	NE,
	SE,
	SW
};

enum DIRECTION {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

enum TURN_DIRECTION {
	LEFT,
	STRAIGHT,
	RIGHT
};

int main() {
    pthread_t car[20];    
    sem_init(&mutex, 0, 1);
    int j;
    for (j = 0; j < 20; j++) {
    	pthread_create(&car[j], NULL, (void *)&handler, &j);
    	pthread_join(car[j], NULL);
  	} 
    sem_destroy(&mutex);     
    return 0;
}

void handler(void *ptr) {
    int x; 
    x = *((int *) ptr);
    printf("Car %d: Waiting to enter critical region\n", x + 1);
    sem_wait(&mutex);
		printf("        Enters critical region\n", x + 1);
		counter++;
		printf("        Increments counter to %d\n", x, counter);
		printf("        Leaves critical region\n", x + 1);
    sem_post(&mutex);
    pthread_exit(0);
}
