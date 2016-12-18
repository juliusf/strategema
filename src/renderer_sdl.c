#include "renderer_sdl.h"
#include "gfx.h"
#include <SDL2/SDL.h>
#include <signal.h>

void clear_render_screen();

void init_sdl_renderer(){
    if (SDL_Init(SDL_INIT_VIDEO| SDL_INIT_NOPARACHUTE) != 0) {
    fprintf(stderr,
            "\nUnable to initialize SDL:  %s\n",
            SDL_GetError()
           );
    }
    atexit(SDL_Quit);
    signal(SIGINT, SIG_DFL); 
    for (int x = 0 ; x < RES_X; x++){
        for (int y= 0; y < RES_Y; y++){
            display_buffer[x][y].x = x * 10;
            display_buffer[x][y].y = y * 10;
            display_buffer[x][y].w = 10;
            display_buffer[x][y].h = 10;
        }
    }
}
void create_window(){
    window = NULL;
    renderer = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Strategema", WIN_INITIAL_X, WIN_INITIAL_Y, WIN_RES_X, WIN_RES_Y, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    clear_render_screen();
}


void clear_render_screen(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, NULL);
    SDL_RenderPresent(renderer); 
}

void update_frame(Gfx* gfx){
    for (int y = 0; y < RES_Y; y++){
        for (int x = 0; x < RES_X; x++){
            if(gfx->back_buffer[x][y]){
                SDL_SetRenderDrawColor( renderer, 0xFF, 0xBF, 0x0, 0xFF );
            }else{
                SDL_SetRenderDrawColor( renderer, 0x0, 0x0, 0x0, 0xFF );
            }
            
            SDL_RenderFillRect( renderer, &display_buffer[x][y] );
        }
    }

    SDL_RenderPresent(renderer);
}

