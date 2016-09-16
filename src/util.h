#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

int32_t read_from_disk(const char* path, uint64_t* length, unsigned char* rom[]);

#endif /* UTIL_H */
