#include "t_math.h"
#include "t_obj.h"

/*

Functions to test:
1. t_decodeOBJ
2. t_rotate
3. t_reorderFace
void t_translate(t_Matrix *in, float translation[3], t_Matrix *out);
void t_transpose(t_Matrix *m1);
void t_genProj(t_Matrix * proj, int* w, int* h, float* FOV);
void t_genView(t_Matrix* viewMatrix, t_Model * model);
void t_genModel(t_Matrix *out, t_Model * model);
void t_project(t_Matrix *proj, t_Matrix *view, t_Matrix *model, t_Face faces[MAX_FACES], t_Face points[MAX_FACES], int faces_count, int WIDTH, int HEIGHT);
void t_drawLine(int x1, int y1, int x2, int y2, uint32_t* pixels, int WIDTH, int HEIGHT);
*/


int main() {
    // 1. obj decoder
    t_Face faces[5];
    const size_t size = sizeof(t_Face);

    t_decodeOBJ("test.obj", faces);
    t_Face face1 = (t_Face) {
        (t_Point) {{ 1.0,  1.0, -1.0, 1.0}},
        (t_Point) {{ 1.0, -1.0, -1.0, 1.0}},
        (t_Point) {{ 1.0,  1.0,  1.0, 1.0}},
        (t_Point) {{ 0.0,  -1.0,  0.0, 1.0}}
    };

    t_Face face2 = (t_Face) {
        (t_Point) {{ 1.0,  1.0,  1.0, 1.0}},
        (t_Point) {{ 1.0, -1.0, -1.0, 1.0}},
        (t_Point) {{ 1.0,  1.0, -1.0, 1.0}},
        (t_Point) {{ -1.0,  0.0,  0.0, 1.0}}
    };

    int test1 = memcmp(&(faces[0]), &face1, size);
    int test2 = memcmp(&(faces[1]), &face2, size);
    if (test1 && test2)
        printf("1: Passed ");
    else 
        printf("1: Failed ");
    printf("t_decodeOBJ test.\n");


    // 2: t_rotate
    t_Matrix x, y, z;
    t_rotate(90, X_AXIS, &x);
    t_rotate(90, Y_AXIS, &y);
    t_rotate(90, Z_AXIS, &z);
    printf("rotate 90 degrees x:\n");
    t_printMatrix(&x);
    printf("\nrotate 90 degrees y:\n");
    t_printMatrix(&y);
    printf("\nrotate 90 degrees z:\n");
    t_printMatrix(&z);

    // 3: t_reorderFace
    // V2 has a backwards normal vector.
    t_reorderFace(&(faces[1]));
    t_Face c_face2 = (t_Face) {
        (t_Point) {{ 1.0,  1.0,  -1.0, 1.0}},
        (t_Point) {{ 1.0, -1.0, -1.0, 1.0}},
        (t_Point) {{ 1.0,  1.0, 1.0, 1.0}},
        (t_Point) {{ -1.0,  0.0,  0.0, 1.0}}
    };
    return 0;
}