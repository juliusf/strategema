#include "renderer_osx.h"
#include "gfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../3rdparty/minifb/MiniFB.h"

void scale_pixel(Gfx* gfx, uint8_t x, uint8_t y, uint32_t color);
void create_window(){
	if (!mfb_open("Strategema - Chip 8", WIN_RES_X, WIN_RES_Y + Y_DRAW_OFFSET))
	{
		printf("Could not create window\n");
		exit(-1);
	}

	for (int i = 0; i < WIN_RES_X * (WIN_RES_Y + Y_DRAW_OFFSET); i++){
		display_buffer[i] = 0;
	}
	int state = mfb_update( display_buffer);
	if (state){
		fprintf(stderr, "unabe to draw screen!\n");
	}
}


void scale_pixel(Gfx* gfx, uint8_t x, uint8_t y, uint32_t color){
	//int draw_offset = 20;
	int pixel_size = 10;
	int scaled_x = x * 10;
	int scaled_y = (y * 10) + Y_DRAW_OFFSET;

	for (int row = scaled_y; row < scaled_y + pixel_size; row++){
		for (int col = scaled_x; col < scaled_x + pixel_size; col++){
			display_buffer[ (row * WIN_RES_X + col) ] = color;
		}
	}
}

void update_frame(Gfx* gfx){
	for (int y = 0; y < RES_Y; y++){

		for (int x = 0; x < RES_X; x++){
			if(gfx->back_buffer[x][y]){
				scale_pixel(gfx, x, y, MFB_RGB(0xFF, 0xBF, 0x0));
			}else{
				scale_pixel(gfx, x, y, MFB_RGB(0x0, 0x0, 0x0));
			}
		}
	}
	int state = mfb_update( display_buffer);
	if (state){
		fprintf(stderr, "unabe to draw screen!\n");
	}

}
