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
void lockSpaces(int turns[4]);
void print_intersection();
char int2char(int i);
void passthrough(int quad,int car_num);

sem_t mutex;
int approach_direction[NUM_CARS];
int turn_direction[NUM_CARS];
int quadrant[4];
sem_t spaces[4];

int main() {
	unsigned int seed = (unsigned int)time(NULL);
	srand(seed);
    pthread_t car[NUM_CARS];
    sem_init(&mutex, 0, 1);
    sem_init(&spaces[0],0,1);
    sem_init(&spaces[1],0,1);
    sem_init(&spaces[2],0,1);
    sem_init(&spaces[3],0,1);
    int j;
    for (j = 1; j <= NUM_CARS; j++) {
    	approach_direction[j] = rand() % 4;
    	turn_direction[j] = rand() % 3;
    	pthread_create(&car[j], NULL, (void *)&handler, &j);
    	pthread_join(car[j], NULL);
  	} 
    sem_destroy(&mutex);     
    return 0;
}

void handler(void *ptr) {
    int x; 
    x = *((int *) ptr);
    int a = approach_direction[x];
    int t = turn_direction[x];
    int turn[4] = {0,0,0,0};
    
    printf("Car %d: Approaching intersection from the %s, wants to turn %s", x, approachToString(a), turnToString(t));
    sem_wait(&mutex);
    	// Critical Region
		printf("        \n", x);
		printf("       Waits for possibility\n");
		
		if (turn[x] == LEFT) {
			turn[(a + 2) % 4] = 1;
			turn[(a + 3) % 4] = 1;
    		turn[a] = 1;
    		lockSpaces(turn);
    		passthrough(a, x);
    		passthrough((a + 3) % 4, x);
    		passthrough((a + 2) % 4, x);
    	} else if (turn[x] == STRAIGHT) {
    		turn[(a + 3) % 4] = 1;
    		turn[a] = 1;
    		lockSpaces(turn);
    		passthrough((a + 3) % 4, x);
    		passthrough(a, x);
    	} else if (turn[x] == RIGHT) {
    		turn[a] = 1;
    		lockSpaces(turn);
    		passthrough(a, x);
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

void passthrough(int quad, int car_num) {
	quadrant[quad] = car_num;
	print_intersection();
	quadrant[quad] = 0;
	sem_post(&spaces[quad]);
}

void lockSpaces(int turns[4]){
	if (turns[0]){
		sem_wait(&spaces[0]);
	}
	if (turns[1]){
		sem_wait(&spaces[1]);
	}
	if (turns[2]){
		sem_wait(&spaces[2]);
	}
	if (turns[3]){
		sem_wait(&spaces[3]);
	}
}
void print_intersection(){
	printf("      │   │   │\n"
"      │   │   │\n"
"      │   │   │\n"
"      │   │   │\n"
"──────┘───│───└──────\n"
"      │ %d │ %d │\n"
"──────────┼──────────\n"
"      │ %d │ %d │\n"
"──────┐───│───┌──────\n"
"      │   │   │\n"
"      │   │   │\n"
"      │   │   │\n"
"      │   │   │\n",int2char(quadrant[0]),int2char(quadrant[1]),int2char(quadrant[3]),int2char(quadrant[2]));
}
char int2char(int i){
	return (i==0) ? ' ' : ('0'+i);
}
