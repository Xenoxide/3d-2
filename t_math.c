#include "t_math.h"

// out = m1*m2
void t_multiMM(t_Matrix* m1, t_Matrix* m2, t_Matrix* out) {

    int i,j,k;
    float s;

    for(i = 0; i < 4; i++) for(j = 0; j < 4; j++) {
        s = 0.0;
        for(k=0; k<4; k++) 
            s += m1->m[i][k] * m2->m[k][j];
        
        out->m[i][j]=s;
    }
}

void t_multiMV(t_Matrix * m1, t_Point * v1, t_Point * out)
{
    int i, j;
    for (i = 0; i < 3; i++)
    {
        out->m[i] = 0.0;
        for (j = 0; j < 3; j++)
        {
            out->m[i] += m1->m[i][j] * v1->m[i];
        };
    }
}
void t_rotate(float angle, int axis, t_Matrix* out) {
    float s = sinf(angle * C);
    float c = cosf(angle * C);

    t_Matrix *tmp;
    memcpy(tmp, &IDENTITY, sizeof(t_Matrix));

    // Construct the rotation matrices by substituting
    // into the identity matrix
    switch (axis) {
        case X_AXIS:
            tmp->m[1][1] =  c; tmp->m[1][2] = -s;
            tmp->m[2][1] =  s; tmp->m[2][2] =  c;
            break;

        case Y_AXIS:
            // Right handed
            tmp->m[0][0] =  c; tmp->m[0][2] =  s;
            tmp->m[2][0] = -s; tmp->m[2][2] =  c;
            break;

        case Z_AXIS:
            tmp->m[0][0] =  c; tmp->m[0][1] = -s;
            tmp->m[1][0] =  s; tmp->m[1][1] =  c;
            break;
    }
}

// w - width
// h - height
// FOV - field of view (in degrees)
// 1 and 1000 are used for near and far planes
t_Matrix t_genProj(float *w, float *h, float *FOV) {
    float AR, FOV_R, t, b, l, r; // top, bottom, left, right
    float f = 1000.0; // far
    float n = 1.0;    // near

    AR = *w / *h; // aspect ratio
    FOV_R = *FOV * C;
    t = n * tan(FOV_R / 2);
    b = -t;
    r = t * AR;
    l = -r;

    // Simplified, because r + l = 0, and r - l = 2r (same with t, b)
    t_Matrix out = {{
        {n / r, 0,     0,                0},
        {0,     n / t, 0,                0},
        {0,     0,     -(f + n)/(f - n), -2 * f * n / (f - n)},
        {0,     0,     -1,               0}
    }};
    return out;
}