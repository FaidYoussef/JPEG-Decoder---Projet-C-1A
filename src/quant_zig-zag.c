#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


// Quantization table
int quant_table[8][8] = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92},
    {49, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}
};


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
    int i, j;
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

// tests quantization & inverse quantization & inverse zig-zag

int main(void) {

    int block[8][8] = {
        {139, 144, 149, 153, 155, 155, 155, 155},
        {144, 151, 153, 156, 159, 156, 156, 156},
        {150, 155, 160, 163, 158, 156, 156, 156},
        {159, 161, 162, 160, 160, 159, 159, 159},
        {159, 160, 161, 162, 162, 155, 155, 155},
        {161, 161, 161, 161, 160, 157, 157, 157},
        {162, 162, 161, 163, 162, 157, 157, 157},
        {162, 162, 161, 161, 163, 158, 158, 158}
    };

    int qblock[8][8];
    int i, j;
    int block2[8][8];
    int block3[64];
    for (i = 0; i < 64; i++) {
        block3[i] = i;
    }

    quantize(block, qblock);
    inv_quantize(qblock, block2);

    for (i = 0; i < 8; i++) {
        for (j =0; j < 8; j++) {
            printf("%d ", qblock[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    for (i = 0; i < 8; i++) {
        for (j =0; j < 8; j++) {
            printf("%d ", block2[i][j]);
        }
        printf("\n");
    }

    printf("\n");


    inv_zig_zag(block3, block2);

    for (i = 0; i < 8; i++) {
        for (j =0; j < 8; j++) {
            printf("%d ", block2[i][j]);
        }
        printf("\n");
    }

    return 0;
}
