#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <quant_zigzag.h>


//Quantization function using quant_table
int* quantize(int *block, int *quant_table_DC, int *quant_table_AC) {
    int* qblock = malloc(64 * sizeof(int));
    check_memory_allocation((void *) qblock);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if(i==0 && j==0) {
                block[0] = qblock[0] * quant_table_DC[0];
            } else {
                qblock[i * 8 + j] = (block[i * 8 + j] / quant_table_AC[i * 8 + j]);
            }
        }
    }

    return qblock;
}

// Inverse quantization function using quant_table
// qblock : les données qui sont quantifiées
int* inv_quantize(unsigned char *qblock, unsigned char *quant_table_DC, unsigned char *quant_table_AC) {
    int* block = malloc(64 * sizeof(int));
    check_memory_allocation((void *) block);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if(i==0 && j==0) {
                block[0] = qblock[0] * quant_table_DC[0];
            } else {
                block[i * 8 + j] = qblock[i * 8 + j] * quant_table_AC[i * 8 + j];
            }
        }
    }

    return block;
}

// Inverse Zig-Zag function
int ** inv_zig_zag(int *block) {
    int **qblock = (int**) malloc(8 * sizeof(int*));
    check_memory_allocation((void *) qblock);
    for (int i = 0; i < 8; i++) {
        qblock[i] = (int*) malloc(8 * sizeof(int));
        check_memory_allocation((void *) qblock[i]);
    }
    int x = 0;
    int y = 0;
    int dir = 1;

    for (int i = 0; i < 64; i++) {
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

    return qblock;
}

void print_block(int block[8][8]){
    for (int i = 0; i < 8; i++) {
        for (int j =0; j < 8; j++) {
            printf("%d ", block[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
