#define _BSB_SOURCE
#include "timer.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* timer_thread(void* timer);
pthread_mutex_t timer_mutex;
uint8_t value;
pthread_t thread;
void initialize_timer(){
	value = 0;
	timer_mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

	if(pthread_create(&thread, NULL, timer_thread, NULL)){
		fprintf(stderr, "Error creating Timerthread\n");
		return;
	}
}
	
void* timer_thread(void* timer){
	const unsigned int sleep_usecs = SLEEP_MS * 1000;
	while(1){
		usleep(sleep_usecs);

		pthread_mutex_lock(&timer_mutex);
			if (value > 0){
				value -=1;
			}
		pthread_mutex_unlock(&timer_mutex);
	}
}

void set_timer_value(uint8_t val){
	pthread_mutex_lock(&timer_mutex);
		value = val;
	pthread_mutex_unlock(&timer_mutex);
}

uint8_t get_timer_value(){
	uint8_t value_copy = 0;
	pthread_mutex_lock(&timer_mutex);
		value_copy = value;
	pthread_mutex_unlock(&timer_mutex);
	return value_copy;
}
