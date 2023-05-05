#include <stdlib.h>
#include <stdbool.h>

#include <quant_zigzag.h>

bool verbose = 1;

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

    int i, j;
    int qblock[8][8];
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

    return EXIT_SUCCESS;
}
