#include "t_math.h"
#include "t_obj.h"

int main() {
    // 1. obj decoder
    t_Face faces[5];
    const size_t size = sizeof(t_Face);

    t_decodeOBJ("test.obj", faces);
    t_Face face1 = (t_Face) {
        (t_Point) {{ 1.0,  1.0, -1.0, 1.0}},
        (t_Point) {{ 1.0, -1.0, -1.0, 1.0}},
        (t_Point) {{ 1.0,  1.0,  1.0, 1.0}},
        (t_Point) {{ 0.0,  1.0,  0.0, 1.0}}
    };

    t_Face face2 = (t_Face) {
        (t_Point) {{ 1.0,  1.0,  1.0, 1.0}},
        (t_Point) {{ 1.0, -1.0, -1.0, 1.0}},
        (t_Point) {{ 1.0,  1.0, -1.0, 1.0}},
        (t_Point) {{ 0.0,  0.0,  1.0, 1.0}}
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
    t_rotate(360, X_AXIS, &x);
}