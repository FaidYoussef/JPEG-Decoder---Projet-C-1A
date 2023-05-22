#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <extract.h>
#include <utils.h>

#define SIZE 8

void transformXY(int16_t src[SIZE*SIZE], int16_t matA[SIZE*SIZE], int16_t matB[SIZE*SIZE], int16_t matC[SIZE*SIZE], int16_t matD[SIZE*SIZE]);

void transformY(int16_t src[SIZE*SIZE], int16_t matA[SIZE*SIZE], int16_t matB[SIZE*SIZE]);

void transformX(int16_t src[SIZE*SIZE], int16_t matA[SIZE*SIZE], int16_t matB[SIZE*SIZE]);

void print_matrix(int16_t matrix[SIZE*SIZE]);

void stretch_function(struct JPEG *jpeg);
