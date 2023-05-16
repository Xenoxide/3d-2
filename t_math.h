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
} t_Matrix4;

typedef struct {
    float m[3][3];
} t_Matrix3;