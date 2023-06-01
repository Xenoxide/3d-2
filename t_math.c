#include "t_math.h"

// out = m1*m2
void t_multi(t_Matrix* m1, t_Matrix* m2, t_Matrix* out) {

    int i,j,k;
    float s;

    for(i = 0; i < 4; i++) for(j = 0; j < 4; j++) {
        s = 0.0;
        for(k=0; k<4; k++) 
            s += m1->m[i][k] * m2->m[k][j];
        
        out->m[i][j]=s;
    }
}

void t_rotate(float angle, int axis, t_Matrix* out) {
    float s = sinf(angle);
    float c = cosf(angle);

    t_Matrix *tmp;
    *tmp = IDENTITY;

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