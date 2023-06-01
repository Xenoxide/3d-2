#ifndef T_OBJ_H
#define T_OBJ_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "t_math.h"

#define MAX_VERTICES 10000
#define MAX_NORMALS 5000
#define MAX_FACES 5000

// Entire file to implement one function. Useful one though.
int t_decodeOBJ(char* filename, t_Face faces[MAX_FACES]);

#endif