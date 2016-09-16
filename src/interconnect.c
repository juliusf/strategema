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
}

void load_rom(Interconnect* interconnect, uint64_t rom_len, unsigned char* rom){
	memcpy(interconnect->ram + PROGRAM_START, rom, rom_len);
}

uint16_t read_word_from_ram(Interconnect* interconnect, uint16_t addr){
	if ( (addr > 0x1ff) && (addr <= 0xfff) )
	{
		return ((interconnect->ram[addr] << 8) | 
			    (interconnect->ram[addr+1])); 
	} else 
	{
		fprintf(stderr, "Invalid Read from address: %x\n", addr);
		exit(-1);
		return 0;
	}
}

