#include "t_obj.h"

// It takes in a ["f", "1/2/3", "1/2/3", "1/2/3"] 
// and returns a [[1, 2, 3], [1, 2, 3], [1, 2, 3]].
// Only used internally.
static void t_splitFace(char** token_array, int out[3][3]) {
    char copy[17];
    char *token, *saveptr;
    int out_index = 0;

    // Tokenize input.
    int i;
    for (i = 0; i < 3; i++) {
        // i + 1 because 0th term is "f".
        strcpy(copy, token_array[i + 1]);

        // "1/2/3" --> "1"
        token = strtok_r(copy, "/", &saveptr);

        while (token != NULL) {
            // Convert to integer, save to out[i]
            out[i][out_index++] = atoi(token);
            // "2/3" --> "2"    "3" --> "3" 
            token = strtok_r(NULL, "/", &saveptr);
        }
        out_index = 0;
    }
}

// Takes in the filename of the obj file, and puts an
// array of faces in the second argument t_Face* faces.
// t_Face* is not a pointer to a face, but an array of faces.
// Return value is number of faces, zero in case of failure.
int t_decodeOBJ(char* filename, t_Face faces[MAX_FACES]) {

    FILE * file = fopen(filename, "r");
    char line_buffer[80];
    
    char *token;
    char* token_array[6];
    int token_index;

    // Save the values.
    t_Point vertices[MAX_VERTICES];
    t_Point normals[MAX_NORMALS];
    int face_references[4][MAX_FACES];

    // Which one am I on?
    int vertices_index = 0;
    int normals_index = 0;
    int faces_index = 0;

    // Line by line sequence
    while(fgets(line_buffer, 80, file) != NULL) {
        
        // Copy string, otherwise same pointer. rest will be destroyed.
        // It's ok to destroy line_buffer, because it is only used once.
        char* rest = line_buffer;
        token_index = 0;

        // Token by token sequence

        while ((token = strtok_r(rest, " ", &rest))) {
            token_array[token_index++] = token;
        }
        

        // Process lines

        // "v" Vertex
        if(!(strcmp(token_array[0], "v"))) {
            // Haha... don't want to mess up the memory...
            if (vertices_index == MAX_VERTICES) return 0;
            vertices[vertices_index++] = (t_Point) {{
                atof(token_array[1]),
                atof(token_array[2]),
                atof(token_array[3]),
                1.0
            }};
        }

        // "vn" Normal
        if(!(strcmp(token_array[0], "vn"))) {
            if (normals_index == MAX_NORMALS) return 0;
            normals[normals_index++] = (t_Point) {{
                atof(token_array[1]),
                atof(token_array[2]),
                atof(token_array[3]),
                1.0
            }};
        }

        // "f" Face (3 sides)
        // 0 - normal
        // 1, 2, 3 - vertices
        // Returns integers instead of t_Point, it will
        // be processed later.
        // Remember: Vertex/(ignore for now)/Vertex normal
        if (!(strcmp(token_array[0], "f"))) {
            int references[3][3];
            t_splitFace(token_array, references);

            // normal (same for all vertices, so use first one)
            face_references[0][faces_index] = references[0][2]; // 2nd term, 0th token
            face_references[1][faces_index] = references[0][0];
            face_references[2][faces_index] = references[1][0];
            face_references[3][faces_index] = references[2][0];
            faces_index++;
        }

        // End processing lines
    }
    // Leave line processing sequence

    // Associate points with faces.
    int j;
    for (j = 0; j < faces_index; j++) {
        faces[j] = (t_Face) { // faces is the output
            vertices[face_references[1][j]],
            vertices[face_references[2][j]],
            vertices[face_references[3][j]],
             normals[face_references[0][j]]
        };
    }

    fclose(file);
    return faces_index + 1;

}