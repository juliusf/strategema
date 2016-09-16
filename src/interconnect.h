
#ifndef INTERCONENCT_H
#define INTERCONENCT_H

#define RAM_SIZE 4 * 1024
#define PROGRAM_START 0x200

#include <inttypes.h>

typedef struct interconnect_t{
	unsigned char ram[RAM_SIZE];
} Interconnect;

void initialize_interconnect(Interconnect** interconnect);
void load_rom(Interconnect* Interconnect, uint64_t rom_len, unsigned char* rom);

#endif /* INTERCONENCT_H */