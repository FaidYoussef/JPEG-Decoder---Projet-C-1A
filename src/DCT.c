#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <math.h>



// Inverse Discrete Cosine Transform function
void inv_dct(int dct_block[8][8], int block[8][8]) {
    int i, j, u, v;
    double cu, cv, sum;
    for (i = 0; i < 8; i++) {
        for (j =0; j < 8; j++) {
            sum = 0;
            for (u = 0; u < 8; u++) {
                cu = (u == 0) ? 1 / sqrt(2) : 1;
                for (v = 0; v < 8; v++) {
                    cv = (v == 0) ? 1 / sqrt(2) : 1;
                    sum += cu * cv * cos((2 * i + 1) * u * M_PI / 16) * cos((2 * j + 1) * v * M_PI / 16);
                }
            }
            block[i][j] = round(sum / 4);
        }
    }
}


// Optimized Inverse Discrete Cosine Transform function
// void inv_dct_opt(int dct_block[8][8], int block[8][8]) {
//     int i, j, u, v;
//     double cu, cv, sum;
//     double cos_u[8], cos_v[8];
//     for (u = 0; u < 8; u++) {
//         cu = (u == 0) ? 1 / sqrt(2) : 1;
//         cos_u[u] = cos((2 * u + 1) * M_PI / 16) * cu;
//     }
//     for (v = 0; v < 8; v++) {
//         cv = (v == 0) ? 1 / sqrt(2) : 1;
//         cos_v[v] = cos((2 * v + 1) * M_PI / 16) * cv;
//     }
//     for (i = 0; i < 8; i++) {
//         for (j =0; j < 8; j++) {
//             sum = 0;
//             for (u = 0; u < 8; u++) {
//                 for (v = 0; v < 8; v++) {
//                     sum += cos_u[u] * cos_v[v];
//                 }
//             }
//             block[i][j] = round(sum / 4);
//         }
//     }
// }
