#ifndef _QUANT_ZIGZAG_
#define _QUANT_ZIGZAG_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <utils.h>


//Quantization function using quant_table
int* quantize(int *block, int *quant_table_DC, int *quant_table_AC);

// Inverse quantization function using quant_table
int* inv_quantize(unsigned char *qblock, unsigned char *quant_table_DC, unsigned char *quant_table_AC);

// Inverse Zig-Zag function
int ** inv_zig_zag(int *block);

// Display block in matrix format
void print_block(int block[8][8]);

#endif
