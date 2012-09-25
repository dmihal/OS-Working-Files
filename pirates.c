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
int other(int this);

pthread_mutex_t store_customers_mutex = PTHREAD_MUTEX_INITIALIZER;
int seats[2] = {0,0};
int turns[2] = {0,0};

const char *types[] = {"Empty ","Pirate","Ninja "};

main()
{
	pthread_t pirates[NUM_PIRATES];
	pthread_t ninjas[NUM_NINJAS];
	int i;

	//Create Ninjas
	for (i = 0; i < NUM_NINJAS; ++i)
	{
		printf("Making ninja %d\n", i);
		pthread_create(&pirates[i],NULL,&person,NINJA);
	}
	//Create Pirates
	for (i = 0; i < NUM_PIRATES; ++i)
	{
		printf("Making pirate %d\n", i);
		pthread_create(&pirates[i],NULL,&person,PIRATE);
	}
    
	sleep(40);

	printf("\nFinal:\n");
	printSeats();
}
void *person(void *typept){
	// type will be NINJA or PIRATE
	int type = (int) typept;
	int seat;
	while(1){
		pthread_mutex_lock(&store_customers_mutex);
		// Be fair, if you've been taking a lot more turns then the other type, take a break
		if ((turns[type]-turns[other(type)]) > 5)
		{
			pthread_mutex_unlock(&store_customers_mutex);
			continue;
		}
		// Are there any empty seats?
		if (!seats[0] || !seats[1])
		{
			// Do any of the seats have my enemy in them?
			if ((seats[0] && seats[0] != type) || (seats[1] && seats[1] != type))
			{
				pthread_mutex_unlock(&store_customers_mutex);
				continue;
			}
			//Try to enter the store and take a seat
			seat = enterStore(type);
			printSeats();

			// Did you get a seat?
			if (-1 != seat)
			{
				turns[type]++;
				pthread_mutex_unlock(&store_customers_mutex);

				sleep(3);
				
				pthread_mutex_lock(&store_customers_mutex);
				//Give up the seat
				leaveStore(seat);
			}
		}
		pthread_mutex_unlock(&store_customers_mutex);
	}
}
// Find a seat (or dressing room, whatever) in the store and assign the person there
// @return int The seat number assigned
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
// Prints who is in each seat and how many times has a pirate or ninja enterd the store
void printSeats(){
	printf("┌──────┬──────┐\n" );
	printf("│%s│%s│\n", types[seats[0]],types[seats[1]]);
	printf("└──────┴──────┘\n" );
	printf("Pirates: %d turns, Ninjas: %d turns\n", turns[PIRATE],turns[NINJA]);
}
// other(PIRATE)=NINJA, other(NINJA)=PIRATE
int other(int this){
	return ((this-2)*(this-2))+1;
}