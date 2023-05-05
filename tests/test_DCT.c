#include <DCT.h>


// Tests for inv_dct and inv_dct_opt

int main() {
    verbose = 1;
    
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