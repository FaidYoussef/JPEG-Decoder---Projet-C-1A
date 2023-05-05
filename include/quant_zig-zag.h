#ifndef _QUANT_ZIG-ZAG_
#define _QUANT_ZIG-ZAG_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


//Quantization function using quant_table
void quantize(int block[8][8], int qblock[8][8]);


// Inverse quantization function using quant_table
void inv_quantize(int qblock[8][8], int block[8][8]);


// Inverse Zig-Zag function
void inv_zig_zag(int block[64], int qblock[8][8]);

#endif;
