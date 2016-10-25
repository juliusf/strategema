#ifndef GFX_H
#define GFX_H

#define RES_X 64
#define RES_Y 32

#include <stdint.h>
#include "util.h"

typedef struct gfx_t{
	unsigned char back_buffer[RES_X][RES_Y];
	char debug_string[RES_X + 1];
} Gfx;


void initialize_gfx(Gfx** gfx);
uint8_t draw_sprite(Gfx* gfx, uint8_t pos_x, uint8_t pos_y, uint8_t sprite_len, unsigned char sprite[]);
void clear_backbuffer(Gfx* gfx);
#endif /* GFX_H */
