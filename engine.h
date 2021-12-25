#ifndef R_MATH_H
    #define R_MATH_H

    #define SCREEN_WIDTH        1200
    #define SCREEN_HEIGHT       650
    #define ASPECT_RATIO        (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH

    typedef struct vec3d {
        float x, y, z;
    } Vec3D;

    typedef struct triangle {
        Vec3D p[3];
    } Triangle;

    typedef Triangle* Mesh;

    typedef struct mat4x4 {
        float m[4][4];
    } Mat4x4;

    void init_mat(Mat4x4 *m);
    void projection_matrix(Mat4x4 *mat_proj, float near, float far, float fov);
    void mmv(Vec3D *o, Vec3D i, Mat4x4 m);

    // draw
    void draw_triangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3);
    void q_fill_triangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3);
    void fill_triangle(SDL_Renderer *renderer, int x0, int y0, int x1, int y1, int x2, int y2);

#endif