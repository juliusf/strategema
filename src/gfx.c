#include "gfx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../3rdparty/minifb/MiniFB.h"

void draw_screen(Gfx* gfx);
uint8_t draw_pixel(Gfx* gfx, uint8_t x, uint8_t y);
void initialize_gfx(Gfx** gfx){
	*(gfx) = (Gfx*) malloc(sizeof(Gfx));
	memset((*gfx), 0x00, RES_X * RES_Y + RES_X + 1); // HACKY	

		if (!mfb_open("Strategema - Chip 8", RES_X * 10, RES_Y * 10))
			exit(-1);

	int noise, carry, seed = 0xbeef;
	for (;;)
	{
		int i, state;

		for (i = 0; i < RES_X * 10 + RES_Y * 10; ++i)
		{
			noise = seed;
			noise >>= 3;
			noise ^= seed;
			carry = noise & 1;
			noise >>= 1;
			seed >>= 1;
			seed |= (carry << 30);
			noise &= 0xFF;
			(*gfx)->display_buffer[i] = MFB_RGB(noise, noise, noise); 
		}

		state = mfb_update( ((*gfx)->display_buffer));

		if (state < 0)
			break;
	}

}

uint8_t draw_sprite(Gfx* gfx, uint8_t pos_x, uint8_t pos_y, uint8_t sprite_len, unsigned char sprite[]){
	uint8_t collision = FALSE;
	
	for (int i = 0; i < sprite_len; i ++){ // lines of the sprite
		uint8_t current_sprite = (uint8_t) sprite[i];
		uint8_t offset = 0;
		for (unsigned int mask = 0x80; mask != 0; mask >>= 1){
			uint8_t pixel = current_sprite & mask;
			if (pixel ) {
				if (draw_pixel(gfx, pos_x + offset, pos_y + i)){
					collision = TRUE;
				}
			}
			offset++;
		}
	}
	draw_screen(gfx);
	return collision;
}

uint8_t draw_pixel(Gfx* gfx, uint8_t x, uint8_t y){
	if(!gfx->back_buffer[x % RES_X][y % RES_Y]){
		gfx->back_buffer[x % RES_X][y % RES_Y] = TRUE;
		return FALSE;
	}else{
		gfx->back_buffer[x % RES_X][y % RES_Y] = FALSE;
		return TRUE;
	}
}

void clear_backbuffer(Gfx* gfx){

	for(int x = 0; x < RES_X; x++){
		for (int y = 0; y < RES_Y; y++){
			gfx->back_buffer[x][y] = FALSE;
		}
	}
}

void draw_screen(Gfx* gfx){
	for (int y = 0; y < RES_Y; y++){

		for (int x = 0; x < RES_X; x++){
			if(gfx->back_buffer[x][y]){
				gfx->debug_string[x] = 'o';
			}else{
				gfx->debug_string[x] = ' ';
			}
		}
		printf("|%s|%i\n", gfx->debug_string,y);
	}
	printf("--------------------------------\n");
}

