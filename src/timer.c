#define _BSB_SOURCE
#include "timer.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* timer_thread(void* timer);
pthread_mutex_t timer_mutex;
void initialize_timer(Timer** timer){
	(*timer) = (Timer*) malloc(sizeof(Timer));
	if (! (*timer)){
		fprintf(stderr, "Unable to initialize timer!\n");
		return;
	}
	(*timer)->value = 0;
	timer_mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

	if(pthread_create(&((*timer)->thread), NULL, timer_thread, &(*timer))){
		fprintf(stderr, "Error creating Timerthread\n");
		return;
	}
}

void* timer_thread(void* timer){
	Timer* my_timer = (Timer*) timer;
	//pthread_mutex_t my_timer_mutex = (my_timer->mutex);
	const unsigned int sleep_usecs = SLEEP_MS * 1000;
	while(1){
		usleep(sleep_usecs);

		pthread_mutex_lock(&timer_mutex);
			if (my_timer->value > 0){
				my_timer->value -= 1;
			}
		pthread_mutex_unlock(&timer_mutex);
	}
}

void set_timer_value(Timer* timer, uint8_t value){
	pthread_mutex_lock(&timer_mutex);
		timer->value = value;
		printf("Setting Value\n");
	pthread_mutex_unlock(&timer_mutex);
}

uint8_t get_timer_value(Timer* timer){
	uint8_t value_copy = 0;
	pthread_mutex_lock(&timer_mutex);
		value_copy = timer->value;
	pthread_mutex_unlock(&timer_mutex);
	return value_copy;
}
