#include "gfx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void draw_screen(Gfx* gfx);
uint8_t set_pixel(Gfx*gfx, uint8_t X, uint8_t Y);

void initialize_gfx(Gfx** gfx){
	*(gfx) = (Gfx*) malloc(sizeof(Gfx));
	memset((*gfx), 0x00, RES_X * RES_Y);
}

uint8_t draw_sprite(Gfx* gfx, uint8_t pos_x, uint8_t pos_y, uint8_t sprite_len, unsigned char* sprite){
	printf("pos_x: %x pos_y %x \n", pos_x, pos_y);
	uint8_t VX = FALSE;
	for (int i = pos_y ; i < pos_y + sprite_len; i ++){
		if (set_pixel(gfx, i, pos_x)){
			VX =TRUE;
		}
	}
	draw_screen(gfx);
	return VX;
}

void draw_screen(Gfx* gfx){
	for (int line = 0; line < RES_Y; line++){
		char* display_string = (char*) malloc(RES_X + 1);
		for (int col = 0; col < RES_X; col++){
			if(gfx->back_buffer[line][col]){
				display_string[col] = 'O';
			}else{
				display_string[col] = ' ';
			}

		}
		printf("|%s|\n", display_string);
		free(display_string);
	}
	printf("--------------------------------\n");
}

uint8_t set_pixel(Gfx*gfx, uint8_t x, uint8_t y){
	uint8_t pix = gfx->back_buffer[x][y];
	gfx->back_buffer[x][y] = ! pix;
	return !pix;
}
