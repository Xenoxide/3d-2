// WARNING: All vector arithmetic neglects the W component.
// It is not necessary to consider it for this implementation.

// To generate projection matrix, use t_genProj, with the camera parameters
// To generate model matrix, use t_genModel, with the object model
// To generate view matrix, use t_genView, with the camera model

#ifndef T_MATH_H
#define T_MATH_H

// includes
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

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

#define MAX_FACES 5000
#define MAX_VERTICES 10000
#define MAX_NORMALS 5000
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

typedef struct {
    float pos[3];
    float rot[3];
} t_Model;

typedef struct {
    int p[2];
} t_Pixel;

// Function definitions

// debug
void t_printMatrix(t_Matrix* m1);

void t_multiMM(t_Matrix* m1, t_Matrix* m2, t_Matrix* out);
void t_multiMV(t_Matrix* m1, t_Point* v1, t_Point* out);
void t_rotate(float angle, int axis, t_Matrix* out);
void t_vectAdd(t_Point* v1, t_Point* v2, t_Point* out);
void t_vectSub(t_Point* v1, t_Point* v2, t_Point* out);
float t_vectDot(t_Point* v1, t_Point* v2);
void t_vectCross(t_Point* v1, t_Point* v2, t_Point* out);
void t_reorderFace(t_Face *in);
void t_translate(t_Matrix *in, float translation[3]);
void t_transpose(t_Matrix *m1);
void t_genProj(t_Matrix * proj, int* w, int* h, float* FOV);
void t_genView(t_Matrix* viewMatrix, t_Model * model);
void t_genModel(t_Matrix *out, t_Model * model);
void t_project(t_Matrix *proj, t_Matrix *view, t_Matrix *model, t_Face faces[MAX_FACES], t_Face points[MAX_FACES], int faces_count, int WIDTH, int HEIGHT);
void t_drawLine(int x1, int y1, int x2, int y2, uint32_t* pixels, int WIDTH, int HEIGHT);

#endif //ifndef T_MATH_H

// write unit tests