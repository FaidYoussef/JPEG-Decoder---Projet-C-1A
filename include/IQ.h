#ifndef _QUANT_ZIGZAG_
#define _QUANT_ZIGZAG_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <extract.h>
#include <utils.h>


// Inverse quantization function using quant_table
int8_t inv_quantize(struct JPEG * jpeg);

// Display block in matrix format
void print_block(int16_t *block);

#endif
