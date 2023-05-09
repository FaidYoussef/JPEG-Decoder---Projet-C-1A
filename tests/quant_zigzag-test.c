#include <stdlib.h>
#include <stdbool.h>

#include <quant_zigzag.h>

bool verbose = 1;

// tests quantization & inverse quantization & inverse zig-zag
int main(void) {

    // Quantization table
    int quant_table[64] = {16, 11, 10, 16, 24, 40, 51, 61,
                            12, 12, 14, 19, 26, 58, 60, 55,
                            14, 13, 16, 24, 40, 57, 69, 56,
                            14, 17, 22, 29, 51, 87, 80, 62,
                            18, 22, 37, 56, 68, 109, 103, 77,
                            24, 35, 55, 64, 81, 104, 113, 92,
                            49, 64, 78, 87, 103, 121, 120, 101,
                            72, 92, 95, 98, 112, 100, 103, 99};

    int block[64] = {139, 144, 149, 153, 155, 155, 155, 155,
                    144, 151, 153, 156, 159, 156, 156, 156,
                    150, 155, 160, 163, 158, 156, 156, 156,
                    159, 161, 162, 160, 160, 159, 159, 159,
                    159, 160, 161, 162, 162, 155, 155, 155,
                    161, 161, 161, 161, 160, 157, 157, 157,
                    162, 162, 161, 163, 162, 157, 157, 157,
                    162, 162, 161, 161, 163, 158, 158, 158};

    int *qblock;
    int *block2;
    int block3[64];
    int i;
    for (i = 0; i < 64; i++) {
        block3[i] = i;
    }

    printf("Original block:\n");
    // print_block(block);


    qblock = quantize(block, quant_table);
    block2 = inv_quantize(qblock, quant_table);


    printf("Quantized block:\n");
    // print_block(qblock);


    printf("Inverse quantized block:\n");
    // print_block(block2);


    block2 = inv_zig_zag(block3);


    printf("Inverse zig-zag block:\n");
    // print_block(block2);


    return EXIT_SUCCESS;
}
