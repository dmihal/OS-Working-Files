//David Mihal & Dylan Kirby
#include <stdio.h>
#include <pthread.h>

#define PIRATE 1
#define NINJA  2

#define NUM_PIRATES 12
#define NUM_NINJAS  4

void* person(void *typept);
int enterStore(int type);
void leaveStore(int seat);
void printSeats();

pthread_mutex_t store_status_mutex = PTHREAD_MUTEX_INITIALIZER;
int store_status = 0;

pthread_mutex_t store_customers_mutex = PTHREAD_MUTEX_INITIALIZER;
int store_customers = 0;
int seats[2];

const char *types[] = {"Empty ","Pirate","Ninja "};

main()
{
	pthread_t pirates[NUM_PIRATES];
	pthread_t ninjas[NUM_NINJAS];
	int i;
	seats[0]=0;
	seats[1]=0;

	for (i = 0; i < NUM_NINJAS; ++i)
	{
		printf("Making ninja %d\n", i);
		pthread_create(&pirates[i],NULL,&person,NINJA);
	}
	for (i = 0; i < NUM_PIRATES; ++i)
	{
		printf("Making pirate %d\n", i);
		pthread_create(&pirates[i],NULL,&person,PIRATE);
	}
    
	sleep(30);
}
void *person(void *typept){
	printf("Yo\n");
	int type = (int) typept;
	int seat;
	while(1){
		pthread_mutex_lock(&store_customers_mutex);
		if (!seats[0] || !seats[1])
		{
			seat = enterStore(type);
			printSeats();
			pthread_mutex_unlock(&store_customers_mutex);
			if (-1 != seat)
			{
				sleep(3);
				leaveStore(seat);
			}
		} else {
			pthread_mutex_unlock(&store_customers_mutex);
		}
	}
}
int enterStore(int type){
	if (!seats[0])
	{
		seats[0] = type;
		return 0;
	} else if (!seats[1])
	{
		seats[1] = type;
		return 1;
	} else {
		return -1;
	}
}
void leaveStore(int seat){
	seats[seat]=0;
}
void printSeats(){
	printf("┌──────┬──────┐\n" );
	printf("│%s│%s│\n", types[seats[0]],types[seats[1]]);
	printf("└──────┴──────┘\n" );
}
