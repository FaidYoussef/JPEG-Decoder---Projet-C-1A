#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../include/quant_zigzag.h"


//Quantization function using quant_table
void quantize(int block[8][8], int qblock[8][8]) {
    int i, j;
    for (i = 0; i < 8; i++) {
        for (j =0; j < 8; j++) {
            qblock[i][j] = round(block[i][j] / quant_table[i][j]);
        }
    }
}

// Inverse quantization function using quant_table
void inv_quantize(int qblock[8][8], int block[8][8]) {
    int i, j;
    for (i = 0; i < 8; i++) {
        for (j =0; j < 8; j++) {
            block[i][j] = qblock[i][j] * quant_table[i][j];
        }
    }
}

// Inverse Zig-Zag function
void inv_zig_zag(int block[64], int qblock[8][8]) {
    int i;
    int x = 0;
    int y = 0;
    int dir = 1;

    for (i = 0; i < 64; i++) {
        qblock[y][x] = block[i];
        if (dir == 1) {
            if (x == 7) {
                y++;
                dir = -1;
            } else if (y == 0) {
                x++;
                dir = -1;
            } else {
                x++;
                y--;
            }
        } else {
            if (y == 7) {
                x++;
                dir = 1;
            } else if (x == 0) {
                y++;
                dir = 1;
            } else {
                x--;
                y++;
            }
        }
    }
}
