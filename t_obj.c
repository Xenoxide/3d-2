#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "t_math.h"

#define MAX_VERTICES 10000
#define MAX_NORMALS 5000
#define MAX_FACES 5000

// It takes in a "1/2/3" and returns a [1, 2, 3].
void t_splitFace(char* in, int* out) {
    char copy[6];
    char *token, *saveptr;

    strcpy(copy, in);
    token = strtok_r(copy, "/", &saveptr);

    while (token != NULL) {
        int number = atoi(token);
        printf("%d\n", number);

        token = strtok_r(NULL, "/", &saveptr);
    }
}

// Takes in the filename of the obj file, and puts an
// array of faces in the second argument t_Face* faces.
// t_Face* is not a pointer to a face, but an array of faces.
void t_decodeOBJ(char* filename, t_Face* faces) {

    FILE * file = fopen(filename, "r");
    char line_buffer[80];
    
    char* token;
    char* rest;
    char* token_array[5];
    int token_index;

    t_Point vertices[MAX_VERTICES];
    t_Point normals[MAX_NORMALS];
    int face_references[4][MAX_FACES];

    int vertices_index = 0;
    int normals_index = 0;
    int faces_index = 0;

    // Line by line sequence
    while(fgets(line_buffer, 80, file)) {

        rest = line_buffer;
        token_index = 0;

        // Token by token sequence
        while ((token = strtok_r(rest, " ", &rest))) {
            printf("%s\n", token);
            token_array[token_index++] = token;
        }

        // Process lines

        // "v" Vertex
        if(!(strcmp(token_array[0], "v"))) {
            // Haha... don't want to mess up the memory...
            if (vertices_index == MAX_VERTICES) return;
            vertices[vertices_index++] = (t_Point) {
                atof(token_array[1]),
                atof(token_array[2]),
                atof(token_array[3]),
                1
            };
        }

        // "vn" Normal
        if(!(strcmp(token_array[0], "vn"))) {
            if (normals_index == MAX_NORMALS) return;
            normals[normals_index++] = (t_Point) {
                atof(token_array[1]),
                atof(token_array[2]),
                atof(token_array[3]),
                1
            };
        }

        // "f" Face (3 sides)
        // 0 - normal
        // 1, 2, 3 - vertices
        // Returns integers instead of t_Point, it will
        // be processed later.

        if (!(strcmp(token_array[0], "f"))) {
            char* copy;
            strcpy(copy, token_array[1]);
            
        }
    }
    fclose(file);

}

int main() {
    t_Face * face;
    t_decodeOBJ("cube.txt", face);
    return 0;
}