#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <IDCT.h>


double C[N];
double C_cos_values[N][N][N][N];

void initialize() {
    FILE *file;

    for (int i = 0; i < N; i++) {
        C[i] = (i == 0) ? 0.5 / sqrt(2) : 0.5;
    }
    

    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            for (int u = 0; u < N; u++) {
                for (int v = 0; v < N; v++) {
                    C_cos_values[x][y][u][v] = C[u] * C[v] cos(((2 * x + 1) * u * PI) / (2 * N)) * cos(((2 * y + 1) * v * PI) / (2 * N));
                }
            }
        }
    }
    file = fopen("C_cos_values.dat", "wb");
    fwrite(C_cos_values, sizeof(double), N*N*N*N, file);
    fclose(file);
}

void load() {
    FILE *file;

    file = fopen("C_cos_values.dat", "rb");
    fread(C_cos_values, sizeof(double), N*N*N*N, file);
    fclose(file);
}

int * idct(int input[NN]) {
    int *output = (int *) malloc(NN * sizeof(int));
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) { 
            double sum = 0.0;

            for (int u = 0; u < N; u++) {
                for (int v = 0; v < N; v++) {
                    int dct_uv = input[u * N + v];
                    sum +=  dct_uv * C_cos_values[x][y][u][v];
                }
            }
            output[x * N + y] = round(sum);
        }
    }

    return output;
}

