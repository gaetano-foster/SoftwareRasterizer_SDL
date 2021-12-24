#ifndef R_MATH_H
    #define R_MATH_H

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

    void init_mat(Mat4x4 *m) 
    {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                m->m[c][r] = ( r == c ? 1.0f : 0.0f);
            }
        }
    }

#endif