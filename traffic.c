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
#include <time.h> 

#define NUM_CARS 5

// Approach directions
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

// Turn directions
#define LEFT 0
#define STRAIGHT 1
#define RIGHT 2

// Quadrants
#define NW 0
#define NE 1
#define SE 2
#define SW 3

void handler (void *ptr);
char* approachToString(int n);
char* turnToString(int n);

sem_t mutex;
int approach[NUM_CARS];
int turn[NUM_CARS];

int main() {
	unsigned int seed = (unsigned int)time(NULL);
	srand(seed);
    pthread_t car[NUM_CARS];
    sem_init(&mutex, 0, 1);
    int j;
    for (j = 0; j < NUM_CARS; j++) {
    	approach[j] = rand() % 4;
    	turn[j] = rand() % 3;
    	pthread_create(&car[j], NULL, (void *)&handler, &j);
    	pthread_join(car[j], NULL);
  	} 
    sem_destroy(&mutex);     
    return 0;
}

void handler(void *ptr) {
    int x; 
    x = *((int *) ptr);
    int a = approach[x];
    int t = turn[x];
    
    printf("Car %d: Approaching intersection from the %s, wants to turn %s", x, approachToString(a), turnToString(t));
    sem_wait(&mutex);
    	// Critical Region
		printf("        \n", x);
		printf("       Waits for possibility\n");
		
		if (turn[x] == LEFT) {
			// Lock (a + 2) % 4
			// Lock (a + 3) % 4
    		// Lock a
    	} else if (turn[x] == STRAIGHT) {
    		// Lock (a + 3) % 4
    		// Lock a
    	} else if (turn[x] == RIGHT) {
    		// Lock a
    	}
		
		printf("       Turns %s\n", turnToString(turn[x]));
    sem_post(&mutex);
    pthread_exit(0);
}

char* approachToString(int n) {
	if (n == NORTH)
		return "North";
	if (n == EAST)
		return "East";
	if (n == SOUTH)
		return "South";
	if (n == WEST)
		return "West";
}

char* turnToString(int n) {
	if (n == LEFT)
		return "Left";
	if (n == STRAIGHT)
		return "Straight";
	if (n == RIGHT)
		return "Right";
}

