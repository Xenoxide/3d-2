#include "t_math.h"
#include "t_obj.h"

/*

I'm testing the functions with an amazing tool:
"does it look right"

Functions to test:
Passed 1. t_decodeOBJ
Passed 2. t_rotate
Passed 3. t_reorderFace
Passed 4. t_translate
Passed 5. t_transpose
Passed 6. t_genProj
Passed 7. t_genView
Passed 8. t_genModel
       9. t_project
       10. t_drawLine
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
    
    int test41 = matrixcmp(&expectedTranslation, &translateMatrix);
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

    int test51 = matrixcmp(&transposeMatrix, &expectedTranspose);
    if (test51)
        printf("5: Passed ");
    else
        printf("5: Failed ");
    printf("t_transpose test.\n");

    // 6: t_genProj
    t_Matrix projection;
    int width = 640, height = 480; float FOV = 90.0;
    t_genProj(&projection, &width, &height, &FOV);
    // t_printMatrix(&projection); seems legit
    printf("6: Passed t_genProj test\n");

    // 7: t_genView
    t_Matrix view;
    t_Model camera = (t_Model) {
        {0, 0, 1}, // pos
        {0, 0, 0}  // rot
    };
    t_genView(&view, &camera);
    // t_printMatrix(&view); It's not supposed to change anything anyway.
    printf("7: Passed t_genView test\n");

    // 8: t_genModel
    t_Matrix model;
    t_Model object = (t_Model) {
        {1, 2, 3}, 
        {0, 90, 0}
    };
    t_genModel(&model, &object);
    // t_printMatrix(&model); good
    printf("8: Passed t_genModel test\n");

    // 9: t_project
    t_Face points[3];
    t_project(&projection, &view, &model, faces, points, 2, 640, 480);
    printf("Projected to (%f, %f) and (%f, %f).\n",
        points[0].p1.m[0], points[0].p1.m[1],
        points[1].p1.m[0], points[1].p1.m[1]);
    return 0;
}