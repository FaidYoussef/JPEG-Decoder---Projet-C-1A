#include <stdio.h>
#include <math.h>
#include <IDCT.h>

#define PI 3.14159265358979323846
#define N 8

// C function in the iDCT formula
double C(int x) {
    return (x == 0) ? 1 / sqrt(2) : 1;
}

// Inverse Discrete Cosine Transform function
void idct(double input[N][N], double output[N][N]) {
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            double sum = 0.0;

            for (int u = 0; u < N; u++) {
                for (int v = 0; v < N; v++) {
                    double cu = C(u);
                    double cv = C(v);
                    double dct_uv = input[u][v];
                    double cos_xu = cos(    (double)(   ((float)(2 * x + 1) * u * PI) / (2 * N) )   );
                    double cos_yv = cos(    (double)(   ((float)(2 * y + 1) * v * PI) / (2 * N) )   );
                    sum += cu * cv * dct_uv * cos_xu * cos_yv;
                }
            }

            output[x][y] = round(sum * 0.25);
        }
    }
}
