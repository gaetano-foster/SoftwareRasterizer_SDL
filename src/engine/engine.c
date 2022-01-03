#include "engine.h"

/* Source for fill_triangle: https://www.avrfreaks.net/sites/default/files/triangles.c */

void init_mat(Mat4x4 *m)
{
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            m->m[c][r] = ( r == c ? 1.0f : 0.0f);
        }
    }
}

void projection_matrix(Mat4x4 *mat_proj, float near, float far, float fov) 
{
    float fov_rad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

    init_mat(mat_proj);
    mat_proj->m[0][0] = ASPECT_RATIO * fov_rad;
    mat_proj->m[1][1] = fov_rad;
    mat_proj->m[2][2] = far / (far - near);
    mat_proj->m[3][2] = (-far * near) / (far - near);
    mat_proj->m[2][3] = 1.0f;
    mat_proj->m[3][3] = 0.0f;
}

/* Multiply a vector and a matrix */
void mmv(Vec3D *o, Vec3D i, Mat4x4 m) 
{
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

/* Horizontal line */
void lcd_hline(SDL_Renderer *renderer, int x1, int x2, int y) 
{
	if (x1 >= x2) 
        SWAP(x1, x2);

	for (; x1 <= x2 ; x1++) 
        SDL_RenderDrawPoint(renderer, x1, y);
}

// Fill a triangle
void fill_triangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3) 
{
	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	int changed1 = 0;
	int changed2 = 0;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;

    // Sort vertices
	if (y1 > y2) { SWAP(y1,y2); SWAP(x1,x2); }
	if (y1 > y3) { SWAP(y1,y3); SWAP(x1,x3); }
	if (y2 > y3) { SWAP(y2,y3); SWAP(x2,x3); }

	t1x = t2x = x1; 
    y = y1;   // Starting points

	dx1 = (x2 - x1); if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
	dy1 = (y2 - y1);
 
	dx2 = (x3 - x1); if(dx2<0) { dx2=-dx2; signx2=-1; } else signx2=1;
	dy2 = (y3 - y1);
	
	if (dy1 > dx1) {   // swap values
        SWAP(dx1,dy1);
		changed1 = 1;
	}
	if (dy2 > dx2) {   // swap values
        SWAP(dy2,dx2);
		changed2 = 1;
	}
	
	e2 = (dx2>>1);
    // Flat top, just process the second half
    if(y1==y2) goto next;
    e1 = (dx1>>1);
	
	for (int i = 0; i < dx1;) {
		t1xp=0; t2xp=0;
		if(t1x<t2x) { minx=t1x; maxx=t2x; }
		else		{ minx=t2x; maxx=t1x; }
        // process first line until y value is about to change
		while(i<dx1) {
			i++;			
			e1 += dy1;
	   	   	while (e1 >= dx1) {
				e1 -= dx1;
   	   	   	   if (changed1) t1xp=signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
	// Move line
	next1:
        // process second line until y value is about to change
		while (1) {
			e2 += dy2;		
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp=signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
		if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
	   	lcd_hline(renderer, minx, maxx, y);    // Draw line from min to max points found on the y
		// Now increase y
		if(!changed1) t1x += signx1;
		t1x+=t1xp;
		if(!changed2) t2x += signx2;
		t2x+=t2xp;
    	y += 1;
		if(y==y2) break;
		
   }
	next:
	// Second half
	dx1 = (x3 - x2); if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
	dy1 = (y3 - y2);
	t1x=x2;
 
	if (dy1 > dx1) {   // swap values
        SWAP(dy1,dx1);
		changed1 = 1;
	} else changed1 = 0;
	
	e1 = (dx1>>1);
	
	for (int i = 0; i<=dx1; i++) {
		t1xp=0; t2xp=0;
		if(t1x<t2x) { minx=t1x; maxx=t2x; }
		else		{ minx=t2x; maxx=t1x; }
	    // process first line until y value is about to change
		while(i<dx1) {
    		e1 += dy1;
	   	   	while (e1 >= dx1) {
				e1 -= dx1;
   	   	   	   	if (changed1) { t1xp=signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if(i<dx1) i++;
		}
	next3:
        // process second line until y value is about to change
		while (t2x!=x3) {
			e2 += dy2;
	   	   	while (e2 >= dx2) {
				e2 -= dx2;
				if(changed2) t2xp=signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}	   	   
	next4:

		if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
		if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
	   	lcd_hline(renderer, minx, maxx, y);    // Draw line from min to max points found on the y
		// Now increase y
		if(!changed1) t1x += signx1;
		t1x+=t1xp;
		if(!changed2) t2x += signx2;
		t2x+=t2xp;
    	y += 1;
		if(y>y3) return;
	}
}

void draw_triangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3) 
{
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
    SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
}