/*
*   Software rasterizer made in SDL2
*   Github: @gaetano-foster
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "engine/engine.h"

void on_user_create(SDL_Window *window, SDL_Renderer *renderer, Mesh mesh);

int main(int argc, char **argv) 
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Error! Couldn't init everything. %s\n", SDL_GetError());
        exit(0);
    }

    Mat4x4 mat_proj;
    Vec3D camera;
	SDL_Window *window;
	SDL_Renderer *renderer;
    SDL_Event e;

    Mesh mesh = calloc(12, sizeof(Triangle));
    camera = (Vec3D) { 0, 0, 0 };

    /* Game Loop */
    projection_matrix(&mat_proj, 0.1f, 1000.0f, 90.0f);
    on_user_create(window, renderer, mesh);

    window = SDL_CreateWindow("Rasterizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);

    int close_requested = 0;
    float theta = 0;

    /* FPS Cap Vars */

    double delta = 0;
    double elapsed_time;
    int max_fps = 72;
    double time_per_tick;
    clock_t now;
    clock_t last_time;
    clock_t timer = 0;
    float ticks = 0;

    time_per_tick = CLOCKS_PER_SEC / max_fps;
    elapsed_time = 1.0f / max_fps;
    last_time = clock();
    srand(time(0));

    if (!window) {
        fprintf(stderr, "Error! Window could not be created. %s\n", SDL_GetError());
        exit(0);
    }

    while (!close_requested) {
        SDL_PollEvent(&e);

        close_requested = (e.type == SDL_QUIT);
        if (close_requested) {
            break;
        } 
        else {
            now = clock();
            delta += (now - last_time) / time_per_tick;
            timer += now - last_time;
            last_time = now;

            /* Main Game Loop */
            if (delta >= 1) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                SDL_RenderClear(renderer);  // clear screen   

                /* Update Variables */         
            
                Mat4x4 rot_z, rot_x; // rotation matrices
                theta += elapsed_time;

                init_mat(&rot_z); // hardcode z rotation matrix
		        rot_z.m[0][0] = cosf(theta);
		        rot_z.m[0][1] = sinf(theta);
		        rot_z.m[1][0] = -sinf(theta);
		        rot_z.m[1][1] = cosf(theta);
		        rot_z.m[2][2] = 1;
		        rot_z.m[3][3] = 1;

                init_mat(&rot_x); // hardcode x rotation matrix
		        rot_x.m[0][0] = 1;
		        rot_x.m[1][1] = cosf(theta * 0.5f); // different speed to avoid gimbal lock
		        rot_x.m[1][2] = sinf(theta * 0.5f);
		        rot_x.m[2][1] = -sinf(theta * 0.5f);
		        rot_x.m[2][2] = cosf(theta * 0.5f);
		        rot_x.m[3][3] = 1;

                /*  Draw  Here  */

                /* Draw the triangles */
                for (int i = 0; i < 12; i++) {
                    Triangle tri = mesh[i];
                    Triangle tri_proj, tri_trans = tri, tri_rotz, tri_rotzx;

                    // Rotate in Z-Axis
                    for (int n = 0; n < 3; n++)
			            mmv(&tri_rotz.p[n], tri.p[n], rot_z);

			        // Rotate in X-Axis
                    for (int n = 0; n < 3; n++)
			            mmv(&tri_rotzx.p[n], tri_rotz.p[n], rot_x);

                    tri_trans = tri_rotzx;
                    for (int n = 0; n < 3; n++) // translate the triangle
                        tri_trans.p[n].z = tri_rotzx.p[n].z + 3.0f;

                    /* Use Cross-Product to Get Surface Normal */
			        Vec3D normal, line1, line2;
			        line1.x = tri_trans.p[1].x - tri_trans.p[0].x;
			        line1.y = tri_trans.p[1].y - tri_trans.p[0].y;
			        line1.z = tri_trans.p[1].z - tri_trans.p[0].z;

			        line2.x = tri_trans.p[2].x - tri_trans.p[0].x;
			        line2.y = tri_trans.p[2].y - tri_trans.p[0].y;
		        	line2.z = tri_trans.p[2].z - tri_trans.p[0].z;

		        	normal.x = line1.y * line2.z - line1.z * line2.y;
		        	normal.y = line1.z * line2.x - line1.x * line2.z;
		        	normal.z = line1.x * line2.y - line1.y * line2.x;

                    // it's normally normal to normalize a normal
		        	float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
		        	normal.x /= l;
                    normal.y /= l; 
                    normal.z /= l;
                    
                    if  (normal.x * (tri_trans.p[0].x - camera.x) +
                         normal.y * (tri_trans.p[0].y - camera.y) +                                        
                         normal.z * (tri_trans.p[0].z - camera.z) < 0) {

                        /* Illumination */
                        Vec3D light_direction = (Vec3D) { 0.0f, 0.0f, -1.0f }; // spaghetti
                        float ld = sqrtf(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
				        light_direction.x /= ld; 
                        light_direction.y /= ld; 
                        light_direction.z /= ld;

				        // How similar is normal to light direction?
				        float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;

                        SDL_SetRenderDrawColor(renderer, dp * 255, dp * 255, dp * 255, 255);
                        
                        tri_proj = tri_rotzx;
                        for (int n = 0; n < 3; n++) // apply perspective/projection to triangle
                            mmv(&tri_proj.p[n], tri_trans.p[n], mat_proj);

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

                        fill_triangle(renderer, tri_proj.p[0].x, tri_proj.p[0].y,
                                                tri_proj.p[1].x, tri_proj.p[1].y, 
                                                tri_proj.p[2].x, tri_proj.p[2].y);

                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        draw_triangle(renderer, tri_proj.p[0].x, tri_proj.p[0].y,
                                                tri_proj.p[1].x, tri_proj.p[1].y, 
                                                tri_proj.p[2].x, tri_proj.p[2].y);
                    }
                }
        
                /* Stop Drawing */
                SDL_RenderPresent(renderer);
                /* FPS Cap */
                ticks++;
                delta--;
            }

            if (timer >= CLOCKS_PER_SEC) {
    	        elapsed_time = 1.0f / ticks;
                ticks = 0;
                timer = 0;
            }
        }
    }
    free(mesh);

    return 0;
}

void on_user_create(SDL_Window *window, SDL_Renderer *renderer, Mesh mesh) 
{
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