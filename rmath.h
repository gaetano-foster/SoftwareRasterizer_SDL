#ifndef R_MATH_H
    #define R_MATH_H

    typedef struct vec3d {
        float x, y, z;
    } Vec3D;

    typedef struct triangle {
        Vec3D p[3];
    } Triangle;

    typedef struct mesh {
        Triangle *tris;
    } Mesh;

    typedef struct mat4x4 {
        float m[4][4];
    } Mat4x4;

#endif