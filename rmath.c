#include "rmath.h"

void init_mat(Mat4x4 *m)
{
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            m->m[c][r] = ( r == c ? 1.0f : 0.0f);
        }
    }
}