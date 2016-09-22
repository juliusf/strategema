#include "gfx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void draw_screen(Gfx* gfx);
void initialize_gfx(Gfx** gfx){
	*(gfx) = (Gfx*) malloc(sizeof(Gfx));
	memset((*gfx), 0x00, RES_X * RES_Y);
}

uint8_t draw_sprite(Gfx* gfx, uint8_t pos_x, uint8_t pos_y, uint8_t sprite_len, unsigned char* sprite){
	printf("pos_x: %x pos_y %x \n", pos_x, pos_y);
	
	for (int i = pos_y ; i < pos_y + sprite_len; i ++){
		gfx->back_buffer[pos_x][i]= 1;
	}
	draw_screen(gfx);
	return FALSE;
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

