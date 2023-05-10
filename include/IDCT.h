#ifndef _IDCT_H_
#define _IDCT_H_

#include <math.h>
#include <stdio.h>

#include <utils.h>

#define N 8

// C function in the iDCT formula
int C(int x);

// Inverse Discrete Cosine Transform function
int* idct(int input[N * N]);

#endif
