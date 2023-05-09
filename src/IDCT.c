#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <IDCT.h>

#define PI 3.14159265358979323846
#define N 8

// C function in the iDCT formula
int C(int x) {
    return (x == 0) ? 1 / sqrt(2) : 1;
}

// Inverse Discrete Cosine Transform function
int* idct(int input[N * N]) {
    int *output = malloc(N * N * sizeof(int));

    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            int sum = 0;

            for (int u = 0; u < N; u++) {
                for (int v = 0; v < N; v++) {
                    int cu = C(u);
                    int cv = C(v);
                    int dct_uv = input[u * N + v];
                    double cos_xu = cos(    (double)(   ((float)(2 * x + 1) * u * PI) / (2 * N) )   );
                    double cos_yv = cos(    (double)(   ((float)(2 * y + 1) * v * PI) / (2 * N) )   );
                    sum += cu * cv * dct_uv * cos_xu * cos_yv;
                }
            }

            output[x * N + y] = (int) round(sum * 0.25);
        }
    }
    
    return output;
}