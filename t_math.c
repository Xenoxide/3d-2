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

// v1 + v2 = out
void t_vectAdd(t_Point* v1, t_Point* v2, t_Point* out) {
    out->m[_X] = v1->m[_X] + v2->m[_X];
    out->m[_Y] = v1->m[_Y] + v2->m[_Y];
    out->m[_Z] = v1->m[_Z] + v2->m[_Z];
    out->m[_W] = v1->m[_W] + v2->m[_W];
}

// v1 - v2 = out
void t_vectSub(t_Point* v1, t_Point* v2, t_Point* out) {
    out->m[_X] = v1->m[_X] - v2->m[_X];
    out->m[_Y] = v1->m[_Y] - v2->m[_Y];
    out->m[_Z] = v1->m[_Z] - v2->m[_Z];
    out->m[_W] = v1->m[_W] - v2->m[_W];
}

// returns v1 . v2
float t_vectDot(t_Point *v1, t_Point *v2) {
    return v1->m[_X] * v2->m[_X] 
         + v1->m[_Y] * v2->m[_Y]
         + v1->m[_Z] * v2->m[_Z];
         // We are not concerned with w.
}

// v1 x v2 = out
void t_vectCross(t_Point *v1, t_Point *v2, t_Point *out) {
    out->m[_X] = v1->m[_Y] * v2->m[_Z] - v1->m[_Z] * v2->m[_Y];
    out->m[_Y] = v1->m[_Z] * v2->m[_X] - v1->m[_X] * v2->m[_Z];
    out->m[_Z] = v1->m[_X] * v2->m[_Y] - v1->m[_Y] * v2->m[_X];
    out->m[_W] = 1; // Cross product is only for 3-vectors.

}

// Put faces in correct order
void t_reorderFace(t_Face *in) {
    // Formula N = (P2 - P1) x (P3 - P1) to calculate normal.
    t_Point P2mP1;
    t_Point P3mP1;
    t_vectSub(&(in->p2), &(in->p1), &P2mP1);
    t_vectSub(&(in->p3), &(in->p1), &P3mP1);

    t_Point calculated_normal;
    t_vectCross(&P2mP1, &P3mP1, &calculated_normal);

    // If dot product is less than 0, the normal is backwards.
    float dot_product = t_vectDot(&(in->normal), &calculated_normal);
    if(dot_product < 0.0) {
        // This reverses the order of the faces
        t_Point tmp = in->p3;
        in->p3 = in->p1;
        in->p1 = tmp;
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

// substitutes the translation into the matrix:
// [1 0 0 X].
// [0 1 0 Y].
// [0 0 1 Z].
// [0 0 0 1].
void t_translate(t_Matrix *in, t_Point *translation, t_Matrix *out) {
    in->m[_X][3] = translation->m[_X];
    in->m[_Y][3] = translation->m[_Y];
    in->m[_Z][3] = translation->m[_Z];
}

// Generates camera matrix, and regenerates it
void t_genCamera(t_Matrix* cameraMatrix, float rot_x, float rot_y, float rot_z, t_Point *translation) {
    t_Matrix x, y, z, r;
    t_rotate(rot_x, X_AXIS, &x);
    t_rotate(rot_y, X_AXIS, &x);
    t_rotate(rot_z, X_AXIS, &x);

    // Rotation order XYZ.
    t_multiMM(&x, &y, &r);
    t_multiMM(&r, &z, &r);

    t_translate(&r, translation, cameraMatrix);
}