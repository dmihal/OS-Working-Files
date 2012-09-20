//David Mihal & Dylan Kirby
#include <stdio.h>
#include <pthread.h>

#define PIRATE 1
#define NINJA  2

#define NUM_PIRATES 12
#define NUM_NINJAS  4

void* person(void *typept);
void leaveStore();

pthread_mutex_t store_status_mutex = PTHREAD_MUTEX_INITIALIZER;
int store_status = 0;

pthread_mutex_t store_customers_mutex = PTHREAD_MUTEX_INITIALIZER;
int store_customers = 0;

const char *types[] = {"Pirate","Ninja"};

main()
{
	pthread_t pirates[NUM_PIRATES];
	pthread_t ninjas[NUM_NINJAS];
	int i;
	for (i = 0; i < NUM_PIRATES; ++i)
	{
		printf("Making pirate %d\n", i);
		pthread_create(&pirates[i],NULL,&person,PIRATE);
	}
    for (i = 0; i < NUM_NINJAS; ++i)
	{
		printf("Making ninja %d\n", i);
		pthread_create(&pirates[i],NULL,&person,NINJA);
	}
	sleep(30);
}
void *person(void *typept){
	printf("Yo\n");
	int type = (int) typept;
	while(1){
		printf("Wait for Customers\n");
		pthread_mutex_lock(&store_customers_mutex);
		if(store_customers==0){
			store_customers++;
			pthread_mutex_unlock(&store_customers_mutex);
			printf("Wait for Status\n");
			pthread_mutex_lock(&store_status_mutex);
			store_status = type;
			pthread_mutex_unlock(&store_status_mutex);
			printf("%s has entered the store\n", types[type-1]);
			sleep(3);
			leaveStore();
			printf("%s has left the store\n", types[type-1]);
		} else if(store_customers==1) {
			pthread_mutex_unlock(&store_customers_mutex);
			printf("Wait for Status 2\n");
			pthread_mutex_lock(&store_status_mutex);
			if(store_status==type){
				pthread_mutex_unlock(&store_status_mutex);
				enterStore();	
				
				sleep(3);
				leaveStore();
				printf("%s has left the store\n", types[type-1]);
			} else {
				pthread_mutex_unlock(&store_status_mutex);
				pthread_mutex_unlock(&store_customers_mutex);
			}
		}
		printf("Loop\n");
	}
}
void enterStore(){
	printf("Wait for Customers, trying to emter\n");
	pthread_mutex_lock(&store_customers_mutex);
	store_customers++;
	pthread_mutex_unlock(&store_customers_mutex);
}
void leaveStore(){
	printf("Wait for Customers, trying to leave\n");
	pthread_mutex_lock(&store_customers_mutex);
	store_customers--;
	pthread_mutex_unlock(&store_customers_mutex);
}