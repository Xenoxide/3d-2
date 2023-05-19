#include <math.h>
#include "t_math.h"

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

// 3x3 Matrix as input, gives 4x4 matrix with the
// bottom right padded with the identity matrix.
void t_3to4(t_M3* m1, t_M4* out) {
    *out = (t_M4) {{
        {m1->m[0][0], m1->m[0][1], m1->m[0][2], 0},
        {m1->m[1][0], m1->m[1][1], m1->m[1][2], 0},
        {m1->m[2][0], m1->m[2][1], m1->m[2][2], 0},
        {0,           0,           0,           1}
    }};
}

// out = m1*m2
void t_multi3(t_M3* m1, t_M3* m2, t_M3* out) {

    int i,j,k;
    float s;

    for(i = 0; i < 3; i++) for(j = 0; j < 3; j++) {
        s = 0.0;
        for(k=0; k<3; k++) 
            s += m1->m[i][k] * m2->m[k][j];
        
        out->m[i][j]=s;
    }
}

// out = m1*m2
void t_multi4(t_M4* m1, t_M4* m2, t_M4* out) {

    int i,j,k;
    float s;

    for(i = 0; i < 4; i++) for(j = 0; j < 4; j++) {
        s = 0.0;
        for(k=0; k<4; k++) 
            s += m1->m[i][k] * m2->m[k][j];
        
        out->m[i][j]=s;
    }
}

void t_rotate4(float angle, int axis, t_M4* out) {
    float s = sinf(angle);
    float c = cosf(angle);
    t_M4 *tmp;
    *tmp = IDENTITY;

    // Construct the rotation matrices by substituting
    // into the identity matrix
    switch (axis) {
        case X_AXIS:
        tmp->m[1][1] =  c; tmp->m[1][2] = -s;
        tmp->m[2][1] =  s; tmp->m[2][2] =  c;
        break;
        case Y_AXIS:
        tmp->m[0][0] =  c; tmp->m[0][2] =  s;
        tmp->m[2][0] = -s; tmp->m[2][2] =  c;
        break;
        case Z_AXIS:
        tmp->m[0][0] =  c; tmp->m[0][1] =  s;
        tmp->m[1][0] = -s; tmp->m[1][1] =  s;
        break;
    }
}