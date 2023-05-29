// struct definitions

typedef struct {
    float x;
    float y;
    float z;
    float w;
} t_Point;

typedef struct {
    t_Point p1;
    t_Point p2;
    t_Point p3;
//  Hope that there are no quadrilateral faces
    t_Point normal;
} t_Face;

// Matrices are defined left to right, top to bottom
// [0][0]   [0][1]   [0][2]
// [1][0]   [1][1]   [1][2]
// [2][0]   [2][1]   [2][2]
// Remember that arrays are dereferenced in reverse order
typedef struct {
    float m[4][4];
} t_M4;

const t_M4 IDENTITY = (t_M4) {{
    {1, 0, 0, 0}, 
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
}};

// Function definitions

void t_multi(t_M4* m1, t_M4* m2, t_M4* out);
void t_rotate(float angle, int axis, t_M4* out);