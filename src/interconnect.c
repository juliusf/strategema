#include "interconnect.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "gfx.h"

void initialize_interconnect(Interconnect** interconnect){
	*interconnect = (Interconnect*) malloc(sizeof(Interconnect));
	if (!*interconnect){
	fprintf(stderr, "unable to create interconnect\n");
	return;
	}
	printf("initializing %i bytes of RAM\n", RAM_SIZE);
	memset((*interconnect)->ram, 46, RAM_SIZE);
	(*interconnect)->stack_ptr = 0;
	(*interconnect)->gfx = NULL;
	initialize_gfx(&((*interconnect)->gfx));
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

void push_stack(Interconnect* interconnect, uint16_t addr){
	if (interconnect->stack_ptr +1 == STACK_SIZE){
		fprintf(stderr, "Stack Overflow!\n" );
		exit(0);
	}
	interconnect->stack[interconnect->stack_ptr] = addr;
	interconnect->stack_ptr += 1;
}

uint16_t pop_stack(Interconnect* interconnect){
	if(interconnect->stack_ptr == 0){
		fprintf(stderr, "Stack Underflow!\n" );
		exit(0);
	}
	interconnect->stack_ptr -= 1;
	uint16_t addr = interconnect->stack[interconnect->stack_ptr];
	return addr;
}

uint8_t draw_on_screen(Interconnect* interconnect, uint8_t screen_loc_x, uint8_t screen_loc_y, uint8_t memory_loc, uint8_t len){
	return draw_sprite(interconnect->gfx, screen_loc_y, screen_loc_y, len, &(interconnect->ram[memory_loc]));
}
