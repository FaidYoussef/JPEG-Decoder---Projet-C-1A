#ifndef _QUANT_ZIGZAG_
#define _QUANT_ZIGZAG_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <utils.h>


//Quantization function using quant_table
int* quantize(int block[64], int *quant_table);

// Inverse quantization function using quant_table
int* inv_quantize(int qblock[64], int *quant_table);

// Inverse Zig-Zag function
int ** inv_zig_zag(int block[64]);

// Display block in matrix format
void print_block(int **block);

// Display block in matrix format
void print_block(int **block);

// Display block in matrix format
void print_block(int **block);

#endif
