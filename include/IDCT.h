#ifndef _IDCT_H_
#define _IDCT_H_

#include <math.h>
#include <stdio.h>

#include <utils.h>

#define N 8
#define NN N*N


void initialize();

void load();

// Inverse Discrete Cosine Transform function
void idct(int input[NN], int output[NN]);

#endif
