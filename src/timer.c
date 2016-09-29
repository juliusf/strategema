#define _BSB_SOURCE
#include "timer.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* timer_thread(void* timer);

void initialize_timer(Timer** timer){
	(*timer) = (Timer*) malloc(sizeof(Timer));
	if (! (*timer)){
		fprintf(stderr, "Unable to initialize timer!\n");
		return;
	}
	(*timer)->value = 0;
	(*timer)->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

	if(pthread_create(&((*timer)->thread), NULL, timer_thread, &(*timer))){
		fprintf(stderr, "Error creating Timerthread\n");
		return;
	}
}

void* timer_thread(void* timer){
	Timer* my_timer = (Timer*) timer;
	const unsigned int sleep_usecs = SLEEP_MS * 1000;
	while(1){
		usleep(sleep_usecs);

		pthread_mutex_lock(&(my_timer->mutex));
			if (my_timer->value > 0){
				my_timer->value -= 1;
			}
		pthread_mutex_unlock(&(my_timer->mutex));
	}
}

void set_timer_value(Timer* timer, uint8_t value){
	pthread_mutex_lock(&(timer->mutex));
		timer->value = value;
	pthread_mutex_unlock(&(timer->mutex));
}

uint8_t get_timer_value(Timer* timer){
	uint8_t value_copy = 0;
	pthread_mutex_lock(&(timer->mutex));
		value_copy = timer->value;
	pthread_mutex_unlock(&(timer->mutex));
	return value_copy;
}
