#include "interconnect.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void initialize_interconnect(Interconnect** interconnect){
	*interconnect = (Interconnect*) malloc(sizeof(Interconnect));
	if (!*interconnect){
	fprintf(stderr, "unable to create interconnect\n");
	return;
	}
	memset((*interconnect)->ram, 255, RAM_SIZE);

	printf("interconnect RAM at 50: %i\n", (*interconnect)->ram[50]);
}

void load_rom(Interconnect* interconnect, uint64_t rom_len, unsigned char* rom){
	memcpy(interconnect->ram + PROGRAM_START, rom, rom_len);
}

