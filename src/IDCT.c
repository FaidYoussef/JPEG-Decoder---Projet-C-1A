#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <IDCT.h>


double C[N];
double cos_values[N][N][N][N];

void initialize() {
    FILE *file;

    for (int i = 0; i < N; i++) {
        C[i] = (i == 0) ? 1 / sqrt(2) : 1;
    }
    file = fopen("C.dat", "wb");
    fwrite(C, sizeof(double), N, file);
    fclose(file);

    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            for (int u = 0; u < N; u++) {
                for (int v = 0; v < N; v++) {
                    cos_values[x][y][u][v] = cos(((2 * x + 1) * u * PI) / (2 * N)) * cos(((2 * y + 1) * v * PI) / (2 * N));
                }
            }
        }
    }
    file = fopen("cos_values.dat", "wb");
    fwrite(cos_values, sizeof(double), N*N*N*N, file);
    fclose(file);
}

void load() {
    FILE *file;

    file = fopen("C.dat", "rb");
    fread(C, sizeof(double), N, file);
    fclose(file);

    file = fopen("cos_values.dat", "rb");
    fread(cos_values, sizeof(double), N*N*N*N, file);
    fclose(file);
}

int * idct(int input[NN]) {
    int *output = malloc(NN * sizeof(int));
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) { 
            double sum = 0.0;

            for (int u = 0; u < N; u++) {
                double cu = C[u];
                for (int v = 0; v < N; v++) {
                    double cv = C[v];
                    int dct_uv = input[u * N + v];
                    sum += cu * cv * dct_uv * cos_values[x][y][u][v];
                }
            }
            output[x * N + y] = round(sum * 0.25);
        }
    }

    return output;
}

