/*
*   Software rasterizer made in SDL2
*   Github: @gfoster-ptr
*/

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH        1245
#define SCREEN_HEIGHT       720
#define cls()               SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); \
                                                    SDL_RenderClear(renderer) 

int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);

    int close_requested;
	SDL_Window *window;
	SDL_Renderer *renderer;
    SDL_Event e;

    window = SDL_CreateWindow("Rasterizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);

    /* Game Loop */

    while (!close_requested) {
        SDL_PollEvent(&e);
        close_requested = (e.type == SDL_QUIT);

        if (close_requested) {
            break;
        } else {
            cls(); // clear screen
            /*  Draw  Here  */

            
        
            /* Stop Drawing */
            SDL_RenderPresent(renderer);
        }
    }

    return 0;
}