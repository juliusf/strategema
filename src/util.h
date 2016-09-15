#ifndef UTIL_H
#define UTIL_H
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int32_t load_rom(const char* path, uint64_t* length, char* rom[]);

#endif /* UTIL_H */
