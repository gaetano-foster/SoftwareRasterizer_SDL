/*
*   Software rasterizer made in SDL2
*   Github: @gfoster-ptr
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include "rmath.h"

#define SCREEN_WIDTH        1200
#define SCREEN_HEIGHT       650
#define ASPECT_RATIO        (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH

void on_user_create(SDL_Window *window, SDL_Renderer *renderer, Mesh mesh);
void projection_matrix(Mat4x4 *mat_proj, float near, float far, float fov);
void mmv(Vec3D *o, Vec3D i, Mat4x4 m);
void draw_triangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3);

int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);

    Mat4x4 mat_proj;
	SDL_Window *window;
	SDL_Renderer *renderer;
    SDL_Event e;

    Mesh mesh = malloc(12 * sizeof(Triangle));

    /* Game Loop */
    projection_matrix(&mat_proj, 0.1f, 1000.0f, 90.0f);
    on_user_create(window, renderer, mesh);
    window = SDL_CreateWindow("Rasterizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);

    int close_requested = 0;
    while (!close_requested) {
        SDL_PollEvent(&e);
        close_requested = (e.type == SDL_QUIT);

        if (close_requested) {
            break;
        } 
        else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
            SDL_RenderClear(renderer);  // clear screen
            /*  Draw  Here  */

            /* Draw the triangles */
            for (int i = 0; i < 12; i++) {
                Triangle tri = mesh[i];
                Triangle tri_proj = mesh[i];

                for (int n = 0; n < 3; n++)
                    mmv(&tri_proj.p[n], tri.p[n], mat_proj);

                /* Scale mesh into view */
                tri_proj.p[0].x += 1.0f; 
                tri_proj.p[0].y += 1.0f;
			    tri_proj.p[1].x += 1.0f;
                tri_proj.p[1].y += 1.0f;
			    tri_proj.p[2].x += 1.0f; 
                tri_proj.p[2].y += 1.0f;
		    	tri_proj.p[0].x *= 0.5f * (float)SCREEN_WIDTH;
			    tri_proj.p[0].y *= 0.5f * (float)SCREEN_HEIGHT;
		    	tri_proj.p[1].x *= 0.5f * (float)SCREEN_WIDTH;
		    	tri_proj.p[1].y *= 0.5f * (float)SCREEN_HEIGHT;
		    	tri_proj.p[2].x *= 0.5f * (float)SCREEN_WIDTH;
		    	tri_proj.p[2].y *= 0.5f * (float)SCREEN_HEIGHT;

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                draw_triangle(renderer, tri_proj.p[0].x, tri_proj.p[0].y,
                                        tri_proj.p[1].x, tri_proj.p[1].y, 
                                        tri_proj.p[2].x, tri_proj.p[2].y);
            }
        
            /* Stop Drawing */
            SDL_RenderPresent(renderer);
        }
    }
    free(mesh);

    return 0;
}

void on_user_create(SDL_Window *window, SDL_Renderer *renderer, Mesh mesh) {
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

    for (int i = 0; i < 12; i++) {
        mesh[i] = tris[i];
    }
}

void projection_matrix(Mat4x4 *mat_proj, float near, float far, float fov) {
    float fov_rad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159);

    mat_proj->m[0][0] = ASPECT_RATIO * fov_rad;
    mat_proj->m[1][1] = fov_rad;
    mat_proj->m[2][2] = far / (far - near);
    mat_proj->m[3][2] = (-far * near) / (far - near);
    mat_proj->m[2][3] = 1;
    mat_proj->m[3][3] = 0;
}

/* Multiply a vector and a matrix */
void mmv(Vec3D *o, Vec3D i, Mat4x4 m) {
    o->x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	o->y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	o->z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

	if (w != 0.0f) {
		o->x /= w; 
        o->y /= w; 
        o->z /= w;
    }
}

void draw_triangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3) {
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
    SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
}