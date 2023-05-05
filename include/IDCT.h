#ifndef _IDCT_H_
#define _IDCT_H_

#include <math.h>
#include <stdio.h>

#define PI 3.14159265358979323846
#define N 8

// C function in the iDCT formula
double C(int x) {
    return (x == 0) ? 1 / sqrt(2) : 1;
}

// Inverse Discrete Cosine Transform function
void idct(double input[N][N], double output[N][N]);

#endif
