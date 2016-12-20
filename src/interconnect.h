
#ifndef INTERCONENCT_H
#define INTERCONENCT_H

#define RAM_SIZE 4 * 1024
#define STACK_SIZE 16
#define PROGRAM_START 0x200
#define NUM_CHAR_SPRITES 16
#include <inttypes.h>
#include "gfx.h"
typedef struct interconnect_t{
	unsigned char ram[RAM_SIZE];
	uint16_t stack[STACK_SIZE];
	uint16_t stack_ptr;
	uint16_t sprite_lookup_table[NUM_CHAR_SPRITES];
	Gfx* gfx;
} Interconnect;

void initialize_interconnect(Interconnect** interconnect);
void load_rom(Interconnect* Interconnect, uint64_t rom_len, unsigned char* rom);
uint16_t read_word_from_ram(Interconnect* interconnect, uint16_t addr); 
void write_byte_to_ram(Interconnect* interconnect, uint16_t addr, uint8_t value);
void push_stack(Interconnect* interconnect, uint16_t addr);
uint16_t pop_stack(Interconnect* interconnect);
uint8_t draw_on_screen(Interconnect* interconnect, uint8_t screen_loc_x, uint8_t screen_loc_y, uint16_t memory_loc, uint8_t len); 
void clear_screen(Interconnect* interconnect);
#endif /* INTERCONENCT_H */
