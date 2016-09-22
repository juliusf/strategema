
#ifndef INTERCONENCT_H
#define INTERCONENCT_H

#define RAM_SIZE 4 * 1024
#define STACK_SIZE 16
#define PROGRAM_START 0x200

#include <inttypes.h>
#include "gfx.h"
typedef struct interconnect_t{
	unsigned char ram[RAM_SIZE];
	uint16_t stack[STACK_SIZE];
	uint16_t stack_ptr;
	Gfx* gfx;
} Interconnect;

void initialize_interconnect(Interconnect** interconnect);
void load_rom(Interconnect* Interconnect, uint64_t rom_len, unsigned char* rom);
uint16_t read_word_from_ram(Interconnect* interconnect, uint16_t addr); 
void push_stack(Interconnect* interconnect, uint16_t addr);
uint16_t pop_stack(Interconnect* interconnect);
uint8_t draw_on_screen(Interconnect* interconnect, uint8_t screen_loc_x, uint8_t screen_loc_y, uint8_t memory_loc, uint8_t len); 
#endif /* INTERCONENCT_H */
