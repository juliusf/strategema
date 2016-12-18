#ifndef RENDERERSDL_H
#define RENDERERSDL_H

#include "gfx.h"
#include <SDL2/SDL.h>


#define WIN_RES_X RES_X * 10
#define WIN_RES_Y RES_Y * 10  
#define WIN_INITIAL_X = 150
#define WIN_INITIAL_Y = 150

static SDL_Rect display_buffer[RES_X][RES_Y];
static SDL_Renderer* renderer;
static SDL_Window* window; 

void init_sdl_renderer();
void create_window();
void update_frame(Gfx* gfx);


#endif /* RENDERERSDL_H */
