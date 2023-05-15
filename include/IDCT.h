#ifndef _IDCT_H_
#define _IDCT_H_

#include <math.h>
#include <stdio.h>

#include <extract.h>
#include <IQ.h>
#include <utils.h>

#define PI 3.14159265358979323846
#define N 8
#define NN N*N


void initialize();

void load();

// Inverse Discrete Cosine Transform function
int8_t IDCT_function(struct JPEG *jpeg,size_t MCU_number, int8_t component_index);

int8_t IDCT(struct JPEG * jpeg);

#endif
