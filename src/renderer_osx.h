#ifndef RENDEREROSX_H
#define RENDEREROSX_H

#include "gfx.h"

#define WIN_RES_X RES_X * 10
#define WIN_RES_Y RES_Y * 10  
#define Y_DRAW_OFFSET 20

static unsigned int display_buffer[WIN_RES_X * (WIN_RES_Y + Y_DRAW_OFFSET) ];

void create_window();
void update_frame(Gfx* gfx);


#endif /* RENDEREROSX_H */
