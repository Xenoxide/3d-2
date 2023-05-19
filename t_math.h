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

typedef struct {
    float m[4][4];
} t_M4;

typedef struct {
    float m[3][3];
} t_M3;

const t_M4 IDENTITY = (t_M4) {{
    {1, 0, 0, 0}, 
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
}};

// Function definitions

void t_3to4(t_M3* m1, t_M4* out);
void t_multi3(t_M3* m1, t_M3* m2, t_M3* out);
void t_multi4(t_M4* m1, t_M4* m2, t_M4* out);
void t_rotate4(float angle, int axis, t_M4* out);