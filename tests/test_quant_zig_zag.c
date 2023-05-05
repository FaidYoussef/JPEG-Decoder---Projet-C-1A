#include <quant_zig_zag.h>

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

// tests quantization & inverse quantization & inverse zig-zag

int main(void) {
    verbose = 1;

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
