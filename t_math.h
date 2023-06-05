// WARNING: All vector arithmetic neglects the W component.
// It is not necessary to consider it for this implementation.

#ifndef T_MATH_H
#define T_MATH_H

// includes
#include <math.h>
#include <string.h>

// macros
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define _X 0
#define _Y 1
#define _Z 2
#define _W 3

// multiply by to convert degrees to radians
#define C 0.01745278

// struct definitions
// t_Point (vector), t_Matrix

/*
typedef struct {
    float x;
    float y;
    float z;
    float w;
} t_Point;
*/

// This is a better way to represent points. As an array.
typedef struct {
    float m[4];
} t_Point;

// Matrices are defined left to right, top to bottom
// [0][0]   [0][1]   [0][2]
// [1][0]   [1][1]   [1][2]
// [2][0]   [2][1]   [2][2]
// Remember that arrays are dereferenced in reverse order
typedef struct {
    float m[4][4];
} t_Matrix;

typedef struct {
    t_Point p1;
    t_Point p2;
    t_Point p3;
//  Hope that there are no quadrilateral faces
    t_Point normal;
} t_Face;

const t_Matrix IDENTITY = (t_Matrix) {{
    {1.0, 0.0, 0.0, 0.0}, 
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
}};

// Function definitions

void t_multiMM(t_Matrix* m1, t_Matrix* m2, t_Matrix* out);
void t_multiMV(t_Matrix* m1, t_Point* v1, t_Point* out);
void t_rotate(float angle, int axis, t_Matrix* out);
void t_vectAdd(t_Point* v1, t_Point* v2, t_Point* out);
void t_vectSub(t_Point* v1, t_Point* v2, t_Point* out);
float t_vectDot(t_Point* v1, t_Point* v2);
void t_vectCross(t_Point* v1, t_Point* v2, t_Point* out);
void t_reorderFace(t_Face *in);
void t_translate(t_Matrix *in, t_Point *translation, t_Matrix *out);
t_Matrix t_genProj(float* w, float* h, float* FOV);
void t_genCamera(t_Matrix* cameraMatrix, float rot_x, float rot_y, float rot_z, t_Point *translation);

#endif //ifndef T_MATH_H