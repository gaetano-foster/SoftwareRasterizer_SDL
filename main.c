/*
*   Software rasterizer made in SDL2
*   Github: @gfoster-ptr
*/

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "rmath.h"

#define SCREEN_WIDTH        1245
#define SCREEN_HEIGHT       720
#define cls()               SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); \
                                                    SDL_RenderClear(renderer) 

void on_user_create(SDL_Window *window, SDL_Renderer *renderer, Mesh *mesh);

int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);

    int close_requested;
    Mesh *mesh;
    Mesh meshCube;
	SDL_Window *window;
	SDL_Renderer *renderer;
    SDL_Event e;

    /* Game Loop */
    on_user_create(window, renderer, mesh);
    meshCube = *mesh;

    while (!close_requested) {
        SDL_PollEvent(&e);
        close_requested = (e.type == SDL_QUIT);

        if (close_requested) {
            break;
        } 
        else {
            int i;
            cls(); // clear screen
            /*  Draw  Here  */

            for (i = 0; i < 12; i++) {
                Triangle tri = meshCube.tris[i];

                
            }
        
            /* Stop Drawing */
            SDL_RenderPresent(renderer);
        }
    }
    free(mesh);
    free(mesh->tris);

    return 0;
}

void on_user_create(SDL_Window *window, SDL_Renderer *renderer, Mesh *mesh) {
    window = SDL_CreateWindow("Rasterizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    mesh = malloc(sizeof(Mesh) * 12);
    mesh->tris = malloc(12 * sizeof(Triangle));
    
    Triangle tris[12] = {

		// SOUTH
		(Triangle) { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		(Triangle) { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		(Triangle) { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		(Triangle) { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		(Triangle) { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		(Triangle) { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		(Triangle) { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		(Triangle) { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		(Triangle) { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		(Triangle) { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		(Triangle) { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		(Triangle) { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
	};

    mesh->tris = tris;
}