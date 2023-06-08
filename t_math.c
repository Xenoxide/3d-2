#include "t_math.h"
#include <stdint.h>

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

    
    *out = (t_Matrix) {{
    {1.0, 0.0, 0.0, 0.0}, 
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
    }};

    // Construct the rotation matrices by substituting
    // into the identity matrix
    switch (axis) {
        case X_AXIS:
            out->m[1][1] =  c; out->m[1][2] = -s;
            out->m[2][1] =  s; out->m[2][2] =  c;
            break;

        case Y_AXIS:
            // Right handed
            out->m[0][0] =  c; out->m[0][2] =  s;
            out->m[2][0] = -s; out->m[2][2] =  c;
            break;

        case Z_AXIS:
            out->m[0][0] =  c; out->m[0][1] = -s;
            out->m[1][0] =  s; out->m[1][1] =  c;
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
void t_genProj(t_Matrix * proj, int *w, int *h, float *FOV) {
    float AR, FOV_R, t, b, l, r; // top, bottom, left, right
    float f = 1000.0; // far
    float n = 1.0;    // near

    AR = (float) *w / (float) *h; // aspect ratio
    FOV_R = *FOV * C;
    t = n * tan(FOV_R / 2);
//  b = -t;
    r = t * AR;
//  l = -r;

    // Simplified, because r + l = 0, and r - l = 2r (same with t, b)
    t_Matrix out = {{
        {n / r, 0,     0,                0},
        {0,     n / t, 0,                0},
        {0,     0,     -(f + n)/(f - n), -2 * f * n / (f - n)},
        {0,     0,     -1,               0}
    }};

    *proj = out;
}

// Transpose the top left 3x3 submatrix of a 4x4 matrix.
// Matrix is changed.
void t_transpose(t_Matrix *m1) {
    t_Matrix m2 = *m1;

    m1->m[0][1] = m2.m[1][0];
    m1->m[1][0] = m2.m[0][1];
    m1->m[0][2] = m2.m[2][0];
    m1->m[2][0] = m2.m[0][2];
    m1->m[2][3] = m2.m[3][2];
    m1->m[3][2] = m2.m[2][3];
}

// substitutes the translation into the matrix:
// [1 0 0 X].
// [0 1 0 Y].
// [0 0 1 Z].
// [0 0 0 1].
void t_translate(t_Matrix *in, float translation[3], t_Matrix *out) {
    in->m[_X][3] = translation[_X];
    in->m[_Y][3] = translation[_Y];
    in->m[_Z][3] = translation[_Z];
}

// Generates camera matrix, and regenerates it. 
// viewMatrix: pointer to matrix to be regenerated.
// rot_x, rot_y, rot_z: Euler rotations in order XYZ.
// translation: Vector containing x, y, z of camera position.

void t_genModel(t_Matrix *out, t_Model * model) {
    t_Matrix x, y, z, r;

    // Generate rotation matrices
    t_rotate(model->rot[_X], X_AXIS, &x);
    t_rotate(model->rot[_Y], Y_AXIS, &y);
    t_rotate(model->rot[_Z], Z_AXIS, &z);

    // Rotation order XYZ. Combine rotation matrices.
    t_multiMM(&x, &y, &r);
    t_multiMM(&r, &z, &r);

    // Combine translation with rotation matrix
    t_translate(&r, model->pos, out);
}

void t_genView(t_Matrix* viewMatrix, t_Model * model) {
    // Generate model matrix for the camera.
    t_genModel(viewMatrix, model);

    // Invert rotation in viewMatrix using M^T = M^{-1}
    t_transpose(viewMatrix);
    
    // Invert translation by negating coordinates.
    viewMatrix->m[_X][_W] = -viewMatrix->m[_X][_W];
    viewMatrix->m[_Y][_W] = -viewMatrix->m[_Y][_W];
    viewMatrix->m[_Z][_W] = -viewMatrix->m[_Z][_W];
}

void t_divPer(t_Face * in, int WIDTH, int HEIGHT) {
    int W2 = WIDTH / 2;
    int H2 = HEIGHT / 2;

    in->p1.m[_X] /= in->p1.m[_Z];
    in->p1.m[_Y] /= in->p1.m[_Z];
    in->p2.m[_X] /= in->p2.m[_Z];
    in->p2.m[_Y] /= in->p2.m[_Z];
    in->p3.m[_X] /= in->p3.m[_Z];
    in->p3.m[_Y] /= in->p3.m[_Z];

    in->p1.m[_Z] = in->p1.m[_W] = 1;
    in->p2.m[_Z] = in->p2.m[_W] = 1;
    in->p3.m[_Z] = in->p3.m[_W] = 1;

    // NDC --> screen space
    in->p1.m[_X] = in->p1.m[_X] * W2 + W2;
    in->p1.m[_Y] = in->p1.m[_Y] * H2 + H2;
    in->p2.m[_X] = in->p2.m[_X] * W2 + W2;
    in->p2.m[_Y] = in->p2.m[_Y] * H2 + H2;
    in->p3.m[_X] = in->p3.m[_X] * W2 + W2;
    in->p3.m[_Y] = in->p3.m[_Y] * H2 + H2;
}

void t_drawLine(int x1, int y1, int x2, int y2, uint32_t* pixels, int WIDTH, int HEIGHT) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (x1 != x2 || y1 != y2) {
        if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT) {
            pixels[y1 * WIDTH + x1] = 0;  // Set pixel to desired color value
        }
        int err2 = 2 * err;
        if (err2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void t_project(t_Matrix *proj, t_Matrix *view, t_Matrix *model, t_Face faces[MAX_FACES], t_Face out[MAX_FACES], int faces_count, int WIDTH, int HEIGHT) {
    t_Matrix T;
    t_multiMM(model, view, &T);
    t_multiMM(&T, proj, &T);

    int i;
    

    // Apply matrices
    t_Face transformed_faces[MAX_FACES];
    for (i = 0; i < faces_count; i++) {
        t_multiMV(&T, &(faces[i].p1), &(transformed_faces[i].p1));
        t_multiMV(&T, &(faces[i].p2), &(transformed_faces[i].p2));
        t_multiMV(&T, &(faces[i].p3), &(transformed_faces[i].p3));

        // perspective division or something
        t_divPer(&(transformed_faces[i]), WIDTH, HEIGHT);
    }    
    
}