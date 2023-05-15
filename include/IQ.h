#ifndef _QUANT_ZIGZAG_
#define _QUANT_ZIGZAG_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <extract.h>
#include <utils.h>


//Quantization function using quant_table
int* quantize(int *block, int *quant_table_DC, int *quant_table_AC);

// Inverse quantization function using quant_table
int8_t inv_quantize(struct JPEG * jpeg);

// Display block in matrix format
void print_block(int16_t *block);

#endif
