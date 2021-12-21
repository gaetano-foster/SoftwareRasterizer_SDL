#ifndef R_MATH_H
    #define R_MATH_H
    #define vec3(x, y, z)                               (Vec3D) { x, y, z };
    #define tri(x0, y0, z0, x1, y1, z1, x2, y2, z2)     (Triangle) { {vec3(x0, y0, z0), vec3(x1, y1, z1), vec3(x2, y2, z2), } };

    typedef struct vec3d {
        float x, y, z;
    } Vec3D;

    typedef struct triangle {
        Vec3D p[3];
    } Triangle;

    typedef struct mesh {
        Triangle *tris;
    } Mesh;

#endif