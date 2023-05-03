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


// Tests for inv_dct and inv_dct_opt

int main() {
    int i, j;
    int dct_block[8][8];
    int block[8][8];
    int block_opt[8][8];
    int test_block[8][8] = {
        {52, 55, 61, 66, 70, 61, 64, 73},
        {63, 59, 55, 90, 109, 85, 69, 72},
        {62, 59, 68, 113, 144, 104, 66, 73},
        {63, 58, 71, 122, 154, 106, 70, 69},
        {67, 61, 68, 104, 126, 88, 68, 70},
        {79, 65, 60, 70, 77, 68, 58, 75},
        {85, 71, 64, 59, 55, 61, 65, 83},
        {87, 79, 69, 68, 65, 76, 78, 94}
    };
    for (i = 0; i < 8; i++) {
        memcpy(dct_block[i], test_block[i], 8 * sizeof(int));
    }
    inv_dct(dct_block, block);
    inv_dct_opt(dct_block, block_opt);
    for (i = 0; i < 8; i++) {
        for (j =0; j < 8; j++) {
            printf("%d ", block[i][j]);
        }
        printf("\n");

    }

    printf("\n");

    for (i = 0; i < 8; i++) {
        for (j =0; j < 8; j++) {
            printf("%d ", block_opt[i][j]);
        }
        printf("\n");

    }
}