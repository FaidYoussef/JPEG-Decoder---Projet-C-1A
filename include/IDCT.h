#ifndef _IDCT_H_
#define _IDCT_H_

#include <math.h>
#include <stdio.h>

#include <utils.h>

#define PI 3.14159265358979323846
#define N 8
#define NN N*N


void initialize();

void load();

// Inverse Discrete Cosine Transform function
int * idct(int input[NN]);

#endif
