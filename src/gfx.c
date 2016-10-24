#include "gfx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void draw_screen(Gfx* gfx);
uint8_t draw_pixel(Gfx* gfx, uint8_t x, uint8_t y);
void initialize_gfx(Gfx** gfx){
	*(gfx) = (Gfx*) malloc(sizeof(Gfx));
	memset((*gfx), 0x00, RES_X * RES_Y);
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
		char* display_string = (char*) malloc(RES_X + 1);
		if (! display_string){
			printf("could not allocate display_string\n");
			exit(-1);
		}
		for (int x = 0; x < RES_X; x++){
			if(gfx->back_buffer[x][y]){
				display_string[x] = 'o';
			}else{
				display_string[x] = ' ';
			}
		}
		printf("|%s|%i\n", display_string,y);
		free(display_string);
	}
	printf("--------------------------------\n");
}

