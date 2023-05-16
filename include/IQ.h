#ifndef _QUANT_ZIGZAG_
#define _QUANT_ZIGZAG_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <extract.h>
#include <utils.h>

// Fonction qui permet de savoir si on a un overflow
bool isOverflow(int16_t a, int16_t b);

// Inverse quantization function using quant_table
int8_t inv_quantize(struct JPEG * jpeg);

#endif
