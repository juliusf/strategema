#ifndef TIMER_H
#define TIMER_H
#define SLEEP_MS 17
#include <pthread.h>
#include <inttypes.h>

void initialize_timer();
void set_timer_value(uint8_t value);
uint8_t get_timer_value();
#endif /* TIMER_H */
