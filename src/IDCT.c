#include <math.h>
#include <stdio.h>

#define PI 3.14159265358979323846
#define N 8

double C(int x) {
    return (x == 0) ? 1 / sqrt(2) : 1;
}

void idct(double input[N][N], double output[N][N]) {
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            double sum = 0.0;

            for (int u = 0; u < N; u++) {
                for (int v = 0; v < N; v++) {
                    double cu = C(u);
                    double cv = C(v);
                    double dct_uv = input[u][v];
                    double cos_xu = cos(((2 * x + 1) * u * PI) / (2 * N));
                    double cos_yv = cos(((2 * y + 1) * v * PI) / (2 * N));
                    sum += cu * cv * dct_uv * cos_xu * cos_yv;
                }
            }

            output[x][y] = round(sum * 0.25);
        }
    }
}

// int main() {
//     double input[N][N] = {
//         // Mettez ici les coefficients DCT de la matrice 8x8
// 		{0,0,0,0,0,0,0,0},
// 		{1,1,1,1,1,1,1,1},
// 		{2,2,2,2,2,2,2,2},
// 		{3,3,3,3,3,3,3,3},
// 		{4,4,4,4,4,4,4,4},
// 		{5,5,5,5,5,5,5,5},
// 		{6,6,6,6,6,6,6,6},
// 		{7,7,7,7,7,7,7,7}
//     };

//     double output[N][N];
//     idct(input, output);

//     // Affiche la matrice de sortie
//     for (int i = 0; i < N; i++) {
//         for (int j = 0; j < N; j++) {
//             printf("%f ", output[i][j]);
//         }
//         printf("\n");
//     }

//     return 0;
// }
