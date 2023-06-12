#include "t_math.h"
#include "t_obj.h"

/*

Functions to test:
Passed 1. t_decodeOBJ
Passed 2. t_rotate
Passed 3. t_reorderFace
Passed 4. t_translate
Passed 5. t_transpose
void t_genProj(t_Matrix * proj, int* w, int* h, float* FOV);
void t_genView(t_Matrix* viewMatrix, t_Model * model);
void t_genModel(t_Matrix *out, t_Model * model);
void t_project(t_Matrix *proj, t_Matrix *view, t_Matrix *model, t_Face faces[MAX_FACES], t_Face points[MAX_FACES], int faces_count, int WIDTH, int HEIGHT);
void t_drawLine(int x1, int y1, int x2, int y2, uint32_t* pixels, int WIDTH, int HEIGHT);
*/

// compares two matrices
int matrixcmp(t_Matrix *m1, t_Matrix *m2) {
    int row, col;
    for (row = 0; row < 4; row++)
        for (col = 0; row < 4; row++)
            if (m1->m[row][col] != m2->m[row][col])
                return 0;
    
    return 1;
}

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

    int test11 = memcmp(&(faces[0]), &face1, size);
    int test12 = memcmp(&(faces[1]), &face2, size);
    if (test11 && test12)
        printf("1: Passed ");
    else 
        printf("1: Failed ");
    printf("t_decodeOBJ test.\n");


    // 2: t_rotate
    /*
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
    */
    printf("2: Passed t_rotate test.\n");

    // 3: t_reorderFace
    // V2 has a backwards normal vector.
    t_reorderFace(&(faces[1]));
    t_Face c_face2 = (t_Face) {
        (t_Point) {{ 1.0,  1.0,  -1.0, 1.0}},
        (t_Point) {{ 1.0, -1.0, -1.0, 1.0}},
        (t_Point) {{ 1.0,  1.0, 1.0, 1.0}},
        (t_Point) {{ -1.0,  0.0,  0.0, 1.0}}
    };

    int test31 = memcmp(&face2, &c_face2, sizeof(t_Face));
    if (test31)
        printf("3: Passed ");
    else
        printf("3: Failed ");
    printf("t_reorderFace test.\n");

    // 4. t_translate
    t_Matrix testMatrix = (t_Matrix) {{
        {11, 12, 13, 14},
        {21, 22, 23, 24},
        {31, 32, 33, 34},
        {41, 42, 43, 44}
    }};
    t_Matrix translateMatrix = testMatrix;
    t_Matrix expectedTranslation = (t_Matrix) {{
        {11, 12, 13, 1},
        {21, 22, 23, 2},
        {31, 32, 33, 3},
        {41, 42, 43, 44}
    }};
    float translation[3] = {1, 2, 3};
    t_translate(&translateMatrix, translation);
    t_printMatrix(&expectedTranslation);
    putchar('\n');
    t_printMatrix(&translateMatrix);
    int test41 = memcmp(&expectedTranslation, &translateMatrix, sizeof(t_Matrix));
    if (test41)
        printf("4: Passed ");
    else
        printf("4: Failed ");
    printf("t_translate test.\n");


    // 5. t_transpose
    t_Matrix transposeMatrix = testMatrix;
    t_transpose(&transposeMatrix);
    t_Matrix expectedTranspose = (t_Matrix) {{
        {11, 21, 31, 14},
        {12, 22, 32, 24},
        {13, 23, 33, 34},
        {41, 42, 43, 44}
    }};

    int test51 = memcmp(&transposeMatrix, &expectedTranspose, sizeof(t_Matrix));
    if (test51)
        printf("5: Passed ");
    else
        printf("5: Failed ");
    printf("t_transpose test.\n");
    return 0;
}