#include <stdio.h>

#define N 3 // Change this to the size of your matrix

// Function to calculate the determinant of a matrix
double determinant(double matrix[N][N]) {
    double det = 0;
    double submatrix[N-1][N-1];
    int i, j, k, m, n;
    
    if (N == 2) {  // Base case for 2x2 matrix
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    } else {
        for (i = 0; i < N; i++) {
            m = 0;
            n = 0;
            for (j = 1; j < N; j++) {
                for (k = 0; k < N; k++) {
                    if (k != i) {
                        submatrix[m][n] = matrix[j][k];
                        n++;
                        if (n == N-1) {
                            m++;
                            n = 0;
                        }
                    }
                }
            }
            det += matrix[0][i] * determinant(submatrix) * ((i%2 == 0) ? 1 : -1);
        }
    }
    
    return det;
}

int main() {
    double matrix[N][N] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    
    double det = determinant(matrix);
    
    printf("Determinant: %lf\n", det);
    
    return 0;
}