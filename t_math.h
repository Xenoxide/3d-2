#ifndef T_MATH_H
#define T_MATH_H

// includes
#include <math.h>

// macros
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2


// struct definitions
// t_Point (vector), t_Matrix

typedef struct {
    float x;
    float y;
    float z;
    float w;
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
    {1, 0, 0, 0}, 
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
}};

// Function definitions

void t_multi(t_Matrix* m1, t_Matrix* m2, t_Matrix* out);
void t_rotate(float angle, int axis, t_Matrix* out);
#endif