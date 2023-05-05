#ifndef _DCT_H_
#define _DCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <math.h>


// Inverse Discrete Cosine Transform function
void inv_dct(int dct_block[8][8], int block[8][8]);

// Optimized Inverse Discrete Cosine Transform function
// void inv_dct_opt(int dct_block[8][8], int block[8][8]);

#endif;
