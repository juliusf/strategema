#ifndef TIMER_H
#define TIMER_H
#define SLEEP_MS 17
#include <pthread.h>
#include <inttypes.h>

typedef struct timer_t{
	uint8_t value;
	pthread_t thread;
	pthread_mutex_t mutex;
} Timer;

void initialize_timer(Timer** timer);
void set_timer_value(Timer* timer, uint8_t value);
uint8_t get_timer_value(Timer* timer);
#endif /* TIMER_H */
